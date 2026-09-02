#include <QtTest/QtTest>
#include <QDate>
#include <array>
#include <chrono>
#include <memory>
#include "fakevaultrepository.h"
#include "fakevaultsession.h"
#define private public
#include "../src/vaultcontroller.h"
#undef private

#define NO_ID -1

class TestEntry : public Entry
{
public:
    TestEntry(const std::string &notes) : Entry(notes) {}

    std::string getType() const override
    {
        return "TestEntry";
    }
};

static std::unique_ptr<VaultHeader> makeVaultHeader()
{
    std::array<uint8_t, MAGIC_BYTES_SIZE> magicBytes{};
    std::array<uint8_t, ARGON2_SALT_SIZE> argon2Salt{};
    return std::make_unique<VaultHeader>(magicBytes, VAULT_FORMAT_CURRENT_VERSION, argon2Salt, 1,
                                         1);
}

static std::unique_ptr<Persona> makePersona(std::string firstName, std::string lastName)
{
    return std::make_unique<Persona>(std::move(firstName), std::move(lastName),
                                     std::chrono::system_clock::time_point{}, "Address", "000000000");
}

static std::unique_ptr<Website> makeWebsite(std::string notes, std::string title,
                                            std::string username, std::string password, std::string url)
{
    return std::make_unique<Website>(std::move(notes), std::move(title), std::move(username),
                                     std::move(password), std::move(url));
}

static std::unique_ptr<VaultSession> makeVaultSession()
{
    auto vaultSession = std::make_unique<FakeVaultSession>();
    return vaultSession;
}

static std::unique_ptr<VaultSession> makeLinkedPersonaSession(int64_t *personaId,
                                                              int64_t *categoryId, int64_t *targetWebsiteId, int64_t *untouchedWebsiteId)
{
    auto vaultSession = makeVaultSession();

    vaultSession->addPersona(makePersona("Ada", "Lovelace"));
    if (personaId != nullptr)
    {
        *personaId = vaultSession->getPersonas().front()->getId();
    }

    vaultSession->addCategory(std::make_unique<Category>("Passwords"));
    auto &category = vaultSession->getCategories().front();
    if (categoryId != nullptr)
    {
        *categoryId = category->getId();
    }

    auto targetWebsite = makeWebsite("target notes", "Target", "alice", "secret",
                                     "https://target.example.com");
    if (targetWebsiteId != nullptr)
    {
        *targetWebsiteId = targetWebsite->getId();
    }
    category->addEntry(std::move(targetWebsite));

    auto untouchedWebsite = makeWebsite("untouched notes", "Untouched", "bob", "secret",
                                        "https://untouched.example.com");
    if (untouchedWebsiteId != nullptr)
    {
        *untouchedWebsiteId = untouchedWebsite->getId();
    }
    category->addEntry(std::move(untouchedWebsite));

    return vaultSession;
}

static std::unique_ptr<VaultSession> makeLinkedPersonaSessionWithWrongEntry(int64_t *personaId,
                                                                            int64_t *categoryId, int64_t *websiteId, int64_t *wrongEntryId)
{
    auto vaultSession = makeVaultSession();

    vaultSession->addPersona(makePersona("Ada", "Lovelace"));
    if (personaId != nullptr)
    {
        *personaId = vaultSession->getPersonas().front()->getId();
    }

    vaultSession->addCategory(std::make_unique<Category>("Passwords"));
    auto &category = vaultSession->getCategories().front();
    if (categoryId != nullptr)
    {
        *categoryId = category->getId();
    }

    auto website = makeWebsite("website notes", "Target", "alice", "secret",
                               "https://target.example.com");
    if (websiteId != nullptr)
    {
        *websiteId = website->getId();
    }
    category->addEntry(std::move(website));

    auto wrongEntry = std::make_unique<TestEntry>("wrong entry");
    if (wrongEntryId != nullptr)
    {
        *wrongEntryId = wrongEntry->getId();
    }
    category->addEntry(std::move(wrongEntry));

    return vaultSession;
}

static std::unique_ptr<VaultSession> makeVaultSessionWithPersonas(int64_t *firstPersonaId,
                                                                  int64_t *secondPersonaId)
{
    auto vaultSession = makeVaultSession();
    vaultSession->addPersona(makePersona("Ada", "Lovelace"));
    vaultSession->addPersona(makePersona("Grace", "Hopper"));

    if (firstPersonaId != nullptr)
    {
        *firstPersonaId = vaultSession->getPersonas()[0]->getId();
    }
    if (secondPersonaId != nullptr)
    {
        *secondPersonaId = vaultSession->getPersonas()[1]->getId();
    }

    return vaultSession;
}

static std::unique_ptr<VaultSession> makeVaultSessionWithCategoryAndEntry(int64_t *categoryId,
                                                                          int64_t *entryId)
{
    std::unique_ptr<VaultSession> vaultSession = makeVaultSession();
    vaultSession->addCategory(std::make_unique<Category>("Passwords"));
    auto &category = vaultSession->getCategories().front();

    auto entry = std::make_unique<TestEntry>("gmail");
    if (entryId != nullptr)
    {
        *entryId = entry->getId();
    }
    category->addEntry(std::move(entry));

    if (categoryId != nullptr)
    {
        *categoryId = category->getId();
    }

    return vaultSession;
}

static std::unique_ptr<VaultSession> makeVaultSessionWithWebsite(int64_t *categoryId,
                                                                 int64_t *websiteId,
                                                                 int64_t *wrongEntryId = nullptr)
{
    auto vaultSession = makeVaultSession();
    vaultSession->addCategory(std::make_unique<Category>("Passwords"));
    auto &category = vaultSession->getCategories().front();

    auto website = makeWebsite("website notes", "Target", "alice", "secret",
                               "https://target.example.com");
    if (websiteId != nullptr)
    {
        *websiteId = website->getId();
    }
    category->addEntry(std::move(website));

    if (wrongEntryId != nullptr)
    {
        auto wrongEntry = std::make_unique<TestEntry>("wrong entry");
        *wrongEntryId = wrongEntry->getId();
        category->addEntry(std::move(wrongEntry));
    }

    if (categoryId != nullptr)
    {
        *categoryId = category->getId();
    }

    return vaultSession;
}

class VaultControllerTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void openVault_returnsTrue_whenFileExistsAndParsingSucceeds();
    void openVault_returnsFalse_whenFileExistsAndParsingFails();
    void openVault_returnsFalse_whenFileDoesNotExistAndParsingSucceeds();
    void openVault_returnsFalse_whenFileDoesNotExistAndParsingFails();
    void unlockVault_returnsTrue_whenEverythingIsCorrect();
    void unlockVault_returnsFalse_whenPasswordIsWrongButParsingSucceeds();
    void unlockVault_returnsFalse_whenEverythingFailsAndParsingFails();
    void createVault_returnsTrue_whenVaultCreationSucceeds();
    void createVault_returnsFalse_whenVaultCreationFails();
    void lockVault_returnsTrue_whenRepositoryLocksVault();
    void lockVault_returnsFalse_whenRepositoryRefusesToLockVault();
    void lockVault_throws_whenSessionIsNotOpen();
    void getVaultName_returnsVaultName_whenSessionIsOpen();
    void getVaultName_throws_whenSessionIsNotOpen();
    void addCategory_addsCategory_whenSessionIsOpen();
    void addCategory_throws_whenSessionIsNotOpen();
    void getPersonas_returnsPersonas_whenSessionIsOpen();
    void getPersonas_throws_whenSessionIsNotOpen();
    void addPersona_addsPersona_whenSessionIsOpen();
    void addPersona_throws_whenSessionIsNotOpen();
    void getPersonaById_returnsPersona_whenPersonaExists();
    void getPersonaById_throws_whenPersonaDoesNotExist();
    void getPersonaById_throws_whenSessionIsNotOpen();
    void addEntryToCategory_returnsTrue_whenCategoryExists();
    void addEntryToCategory_returnsFalse_whenCategoryDoesNotExist();
    void addEntryToCategory_throws_whenSessionIsNotOpen();
    void getEntriesInCategory_returnsEntries_whenCategoryExists();
    void getEntriesInCategory_returnsEmptyVector_whenCategoryIsEmpty();
    void getEntriesInCategory_returnsEmptyVector_whenCategoryDoesNotExist();
    void getEntriesInCategory_throws_whenSessionIsNotOpen();
    void getWebsitesByUrl_returnsMatchingWebsites_whenSessionIsOpen();
    void getWebsitesByUrl_throws_whenSessionIsNotOpen();
    void getWebsiteById_returnsWebsite_whenWebsiteExists();
    void getWebsiteById_throws_whenWebsiteDoesNotExist();
    void getWebsiteById_throws_whenSessionIsNotOpen();
    void searchEntriesInCategory_returnsMatchingEntries_whenSessionIsOpen();
    void searchEntriesInCategory_returnsEmptyVector_whenNoEntryMatches();
    void searchEntriesInCategory_throws_whenCategoryDoesNotExist();
    void searchEntriesInCategory_throws_whenSessionIsNotOpen();
    void linkPersonaToEntry_returnsTrue_whenPersonaAndEntryExist();
    void linkPersonaToEntry_returnsFalse_whenPersonaDoesNotExist();
    void linkPersonaToEntry_returnsFalse_whenCategoryDoesNotExist();
    void linkPersonaToEntry_returnsFalse_whenEntryDoesNotExist();
    void linkPersonaToEntry_returnsFalse_whenEntryHasWrongType();
    void linkPersonaToEntry_throws_whenSessionIsNotOpen();
    void getCategories_returnsCategories_whenSessionIsOpen();
    void getCategories_returnsEmptyVector_whenSessionIsOpenButEmpty();
    void getCategories_throws_whenSessionIsNotOpen();
    void removeEntryFromCategory_returnsTrue_whenEntryExists();
    void removeEntryFromCategory_returnsFalse_whenCategoryDoesNotExist();
    void removeEntryFromCategory_returnsFalse_whenEntryDoesNotExist();
    void removeEntryFromCategory_throws_whenSessionIsNotOpen();
    void removePersona_returnsTrue_whenPersonaExists();
    void removePersona_returnsFalse_whenPersonaDoesNotExist();
    void removePersona_throws_whenSessionIsNotOpen();
    void setAliasForWebsite_returnsTrue_whenWebsiteExists();
    void setAliasForWebsite_returnsFalse_whenCategoryDoesNotExist();
    void setAliasForWebsite_returnsFalse_whenEntryDoesNotExist();
    void setAliasForWebsite_returnsFalse_whenEntryHasWrongType();
    void setAliasForWebsite_throws_whenSessionIsNotOpen();
};

void VaultControllerTest::init()
{
    VaultController::instance.reset();
}

void VaultControllerTest::cleanup()
{
    VaultController::instance.reset();
}

/**
 * @brief Test the openVault method when the file exists and parsing succeeds.
 */
void VaultControllerTest::openVault_returnsTrue_whenFileExistsAndParsingSucceeds()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY(controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file exists and parsing fails.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileExistsAndParsingFails()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, false);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file does not exist and parsing succeeds.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileDoesNotExistAndParsingSucceeds()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file does not exist and parsing fails.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileDoesNotExistAndParsingFails()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, false);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the unlockVault method when everything is correct.
 */
void VaultControllerTest::unlockVault_returnsTrue_whenEverythingIsCorrect()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QCOMPARE(controller.unlockVault("master-password", "vault"), VaultController::UnlockResult::Success);
    QVERIFY(controller.isVaultUnlocked());
}

/**
 * @brief Test the unlockVault method when the password is wrong but parsing succeeds.
 */
void VaultControllerTest::unlockVault_returnsFalse_whenPasswordIsWrongButParsingSucceeds()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QCOMPARE(controller.unlockVault("wrong-password", "vault"), VaultController::UnlockResult::IncorrectPasswordOrCorrupted);
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test the unlockVault method when everything fails and parsing fails.
 */
void VaultControllerTest::unlockVault_returnsFalse_whenEverythingFailsAndParsingFails()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, false);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QCOMPARE(controller.unlockVault("wrong-password", "vault"), VaultController::UnlockResult::IncorrectPasswordOrCorrupted);
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test the createVault method when vault creation succeeds.
 */
void VaultControllerTest::createVault_returnsTrue_whenVaultCreationSucceeds()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY(controller.createVault("master-password", "vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test the createVault method when vault creation fails.
 */
void VaultControllerTest::createVault_returnsFalse_whenVaultCreationFails()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, false);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY(!controller.createVault("master-password", "vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test lockVault when the repository accepts locking the vault.
 */
void VaultControllerTest::lockVault_returnsTrue_whenRepositoryLocksVault()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = makeVaultSession();

    QVERIFY(controller.lockVault("vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test lockVault when the repository refuses to lock the vault.
 */
void VaultControllerTest::lockVault_returnsFalse_whenRepositoryRefusesToLockVault()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = makeVaultSession();

    QVERIFY(!controller.lockVault("vault"));
    QVERIFY(controller.isVaultUnlocked());
}

/**
 * @brief Test lockVault when no vault session is open.
 */
void VaultControllerTest::lockVault_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.lockVault("vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test getVaultName when a vault session is open.
 */
void VaultControllerTest::getVaultName_returnsVaultName_whenSessionIsOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = std::make_unique<VaultSession>("My Vault", EncKey{}, AuthKey{}, nullptr);

    QCOMPARE(controller.getVaultName(), QString("My Vault"));
}

/**
 * @brief Test getVaultName when no vault session is open.
 */
void VaultControllerTest::getVaultName_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.getVaultName());
}

/**
 * @brief Test addCategory when a vault session is open.
 */
void VaultControllerTest::addCategory_addsCategory_whenSessionIsOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = makeVaultSession();

    controller.addCategory("Passwords");

    QCOMPARE(controller.getCategories().size(), std::size_t(1));
    QCOMPARE(controller.getCategories().front().getName(), QString("Passwords"));
}

/**
 * @brief Test addCategory when no vault session is open.
 */
void VaultControllerTest::addCategory_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             controller.addCategory("Passwords"));
}

/**
 * @brief Test getPersonas when a vault session is open.
 */
void VaultControllerTest::getPersonas_returnsPersonas_whenSessionIsOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = makeVaultSessionWithPersonas(nullptr, nullptr);

    const auto &personas = controller.getPersonas();

    QCOMPARE(personas.size(), std::size_t(2));
    QCOMPARE(personas[0].getFirstName(), QString("Ada"));
    QCOMPARE(personas[1].getFirstName(), QString("Grace"));
}

/**
 * @brief Test getPersonas when no vault session is open.
 */
void VaultControllerTest::getPersonas_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.getPersonas());
}

/**
 * @brief Test addPersona when a vault session is open.
 */
void VaultControllerTest::addPersona_addsPersona_whenSessionIsOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = makeVaultSession();

    controller.addPersona("Ada", "Lovelace", QDate::fromJulianDay(2440588), "Address", "000000000");

    const auto &personas = controller.getPersonas();
    QCOMPARE(personas.size(), std::size_t(1));
    QCOMPARE(personas.front().getFirstName(), QString("Ada"));
}

/**
 * @brief Test addPersona when no vault session is open.
 */
void VaultControllerTest::addPersona_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             controller.addPersona("Ada", "Lovelace", QDate::fromJulianDay(2440588),
                                                   "Address", "000000000"));
}

/**
 * @brief Test getPersonaById when the persona exists.
 */
void VaultControllerTest::getPersonaById_returnsPersona_whenPersonaExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t firstPersonaId = 0;
    int64_t secondPersonaId = 0;
    controller.session = makeVaultSessionWithPersonas(&firstPersonaId, &secondPersonaId);

    const auto persona = controller.getPersonaById(firstPersonaId);

    QCOMPARE(persona.getId(), firstPersonaId);
    QCOMPARE(persona.getFirstName(), QString("Ada"));
}

/**
 * @brief Test getPersonaById when the persona does not exist.
 */
void VaultControllerTest::getPersonaById_throws_whenPersonaDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    controller.session = makeVaultSessionWithPersonas(nullptr, nullptr);

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.getPersonaById(NO_PERSONA_ID));
}

/**
 * @brief Test getPersonaById when no vault session is open.
 */
void VaultControllerTest::getPersonaById_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.getPersonaById(1));
}

/**
 * @brief Test addEntryToCategory when the category exists.
 */
void VaultControllerTest::addEntryToCategory_returnsTrue_whenCategoryExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    controller.removeEntryFromCategory(categoryId, entryId);
    QVERIFY(controller.addEntryToCategory(categoryId, std::make_unique<TestEntry>("gmail")));
    QCOMPARE(controller.getEntriesInCategory(categoryId).size(), std::size_t(1));
}

/**
 * @brief Test addEntryToCategory when the category does not exist.
 */
void VaultControllerTest::addEntryToCategory_returnsFalse_whenCategoryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    QVERIFY(!controller.addEntryToCategory(categoryId + 1, std::make_unique<TestEntry>("gmail")));
    QCOMPARE(controller.getEntriesInCategory(categoryId).size(), std::size_t(1));
}

/**
 * @brief Test addEntryToCategory when no vault session is open.
 */
void VaultControllerTest::addEntryToCategory_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             controller.addEntryToCategory(1, std::make_unique<TestEntry>("gmail")));
}

/**
 * @brief Test getEntriesInCategory when the category exists and contains entries.
 */
void VaultControllerTest::getEntriesInCategory_returnsEntries_whenCategoryExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithWebsite(&categoryId, &entryId);

    const auto &entries = controller.getEntriesInCategory(categoryId);

    QCOMPARE(entries.size(), std::size_t(1));
    QCOMPARE(entries.front()->getId(), entryId);
    QCOMPARE(entries.front()->getKind(), QEntry::EntryKind::Website);
}

/**
 * @brief Test getEntriesInCategory when the category exists but has no entries.
 */
void VaultControllerTest::getEntriesInCategory_returnsEmptyVector_whenCategoryIsEmpty()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = makeVaultSession();
    controller.session->addCategory(std::make_unique<Category>("Passwords"));

    const auto categoryId = controller.session->getCategories().front()->getId();
    const auto &entries = controller.getEntriesInCategory(categoryId);

    QCOMPARE(entries.size(), std::size_t(0));
}

/**
 * @brief Test getEntriesInCategory when the category does not exist.
 */
void VaultControllerTest::getEntriesInCategory_returnsEmptyVector_whenCategoryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    const auto &entries = controller.getEntriesInCategory(categoryId + 1);

    QCOMPARE(entries.size(), std::size_t(0));
}

/**
 * @brief Test getEntriesInCategory when no session is open.
 */
void VaultControllerTest::getEntriesInCategory_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.getEntriesInCategory(1));
}

/**
 * @brief Test getWebsitesByUrl when matching websites exist.
 */
void VaultControllerTest::getWebsitesByUrl_returnsMatchingWebsites_whenSessionIsOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    const auto websites = controller.getWebsitesByUrl("https://target.example.com");

    QCOMPARE(websites.size(), std::size_t(1));
    QCOMPARE(websites.front().getId(), targetWebsiteId);
}

/**
 * @brief Test getWebsitesByUrl when no vault session is open.
 */
void VaultControllerTest::getWebsitesByUrl_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             controller.getWebsitesByUrl("https://target.example.com"));
}

/**
 * @brief Test getWebsiteById when the website exists.
 */
void VaultControllerTest::getWebsiteById_returnsWebsite_whenWebsiteExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    const auto website = controller.getWebsiteById(targetWebsiteId);

    QCOMPARE(website.getId(), targetWebsiteId);
    QCOMPARE(website.getTitle(), QString("Target"));
}

/**
 * @brief Test getWebsiteById when the website does not exist.
 */
void VaultControllerTest::getWebsiteById_throws_whenWebsiteDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, controller.getWebsiteById(NO_ID));
}

/**
 * @brief Test getWebsiteById when no vault session is open.
 */
void VaultControllerTest::getWebsiteById_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.getWebsiteById(1));
}

/**
 * @brief Test searchEntriesInCategory when matching entries exist.
 */
void VaultControllerTest::searchEntriesInCategory_returnsMatchingEntries_whenSessionIsOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    const auto matches = controller.searchEntriesInCategory(categoryId, "target");

    QCOMPARE(matches.size(), std::size_t(1));
    QVERIFY(matches.front() != nullptr);
    QCOMPARE(matches.front()->getId(), targetWebsiteId);
}

/**
 * @brief Test searchEntriesInCategory when no entry matches.
 */
void VaultControllerTest::searchEntriesInCategory_returnsEmptyVector_whenNoEntryMatches()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    const auto matches = controller.searchEntriesInCategory(categoryId, "missing-term");

    QCOMPARE(matches.size(), std::size_t(0));
}

/**
 * @brief Test searchEntriesInCategory when the category does not exist.
 */
void VaultControllerTest::searchEntriesInCategory_throws_whenCategoryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             controller.searchEntriesInCategory(categoryId + 1, "target"));
}

/**
 * @brief Test searchEntriesInCategory when no vault session is open.
 */
void VaultControllerTest::searchEntriesInCategory_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             controller.searchEntriesInCategory(1, "target"));
}

/**
 * @brief Test linkPersonaToEntry when the persona and entry both exist.
 */
void VaultControllerTest::linkPersonaToEntry_returnsTrue_whenPersonaAndEntryExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY(controller.linkPersonaToEntry(personaId, categoryId, targetWebsiteId));

    const auto targetWebsite = controller.getWebsiteById(targetWebsiteId);
    const auto untouchedWebsite = controller.getWebsiteById(untouchedWebsiteId);

    QCOMPARE(targetWebsite.getId(), targetWebsiteId);
    QCOMPARE(targetWebsite.getPersonaId(), personaId);
    QCOMPARE(untouchedWebsite.getId(), untouchedWebsiteId);
    QCOMPARE(untouchedWebsite.getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when the persona does not exist.
 */
void VaultControllerTest::linkPersonaToEntry_returnsFalse_whenPersonaDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY(!controller.linkPersonaToEntry(personaId + 1, categoryId, targetWebsiteId));

    const auto targetWebsite = controller.getWebsiteById(targetWebsiteId);
    QCOMPARE(targetWebsite.getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when the category does not exist.
 */
void VaultControllerTest::linkPersonaToEntry_returnsFalse_whenCategoryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY(!controller.linkPersonaToEntry(personaId, categoryId + 1, targetWebsiteId));

    const auto targetWebsite = controller.getWebsiteById(targetWebsiteId);
    QCOMPARE(targetWebsite.getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when the entry does not exist.
 */
void VaultControllerTest::linkPersonaToEntry_returnsFalse_whenEntryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY(!controller.linkPersonaToEntry(personaId, categoryId, NO_ID));

    const auto targetWebsite = controller.getWebsiteById(targetWebsiteId);
    QCOMPARE(targetWebsite.getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when the targeted entry has a wrong type.
 */
void VaultControllerTest::linkPersonaToEntry_returnsFalse_whenEntryHasWrongType()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t websiteId = 0;
    int64_t wrongEntryId = 0;
    controller.session = makeLinkedPersonaSessionWithWrongEntry(&personaId, &categoryId,
                                                                &websiteId, &wrongEntryId);

    QVERIFY(!controller.linkPersonaToEntry(personaId, categoryId, wrongEntryId));

    const auto website = controller.getWebsiteById(websiteId);
    QCOMPARE(website.getId(), websiteId);
    QCOMPARE(website.getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when no session is open.
 */
void VaultControllerTest::linkPersonaToEntry_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.linkPersonaToEntry(1, 1, 1));
}

/**
 * @brief Test getCategories when the session is open and contains categories.
 */
void VaultControllerTest::getCategories_returnsCategories_whenSessionIsOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = makeVaultSession();
    controller.addCategory("Passwords");
    controller.addCategory("Websites");

    const auto &categories = controller.getCategories();

    QCOMPARE(categories.size(), std::size_t(2));
    QCOMPARE(categories[0].getName(), QString("Passwords"));
    QCOMPARE(categories[1].getName(), QString("Websites"));
}

/**
 * @brief Test getCategories when the session is open but has no categories.
 */
void VaultControllerTest::getCategories_returnsEmptyVector_whenSessionIsOpenButEmpty()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));
    controller.session = makeVaultSession();

    const auto &categories = controller.getCategories();

    QCOMPARE(categories.size(), std::size_t(0));
}

/**
 * @brief Test getCategories when no session is open.
 */
void VaultControllerTest::getCategories_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.getCategories());
}

/**
 * @brief Test removeEntryFromCategory when the entry exists in the category.
 */
void VaultControllerTest::removeEntryFromCategory_returnsTrue_whenEntryExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    QVERIFY(controller.removeEntryFromCategory(categoryId, entryId));
    QCOMPARE(controller.getEntriesInCategory(categoryId).size(), std::size_t(0));
}

/**
 * @brief Test removeEntryFromCategory when the category does not exist.
 */
void VaultControllerTest::removeEntryFromCategory_returnsFalse_whenCategoryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    QVERIFY(!controller.removeEntryFromCategory(categoryId + 1, entryId));
    QCOMPARE(controller.getEntriesInCategory(categoryId).size(), std::size_t(1));
}

/**
 * @brief Test removeEntryFromCategory when the entry does not exist in the category.
 */
void VaultControllerTest::removeEntryFromCategory_returnsFalse_whenEntryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    QVERIFY(!controller.removeEntryFromCategory(categoryId, entryId + 1));
    QCOMPARE(controller.getEntriesInCategory(categoryId).size(), std::size_t(1));
}

/**
 * @brief Test removeEntryFromCategory when no session is open.
 */
void VaultControllerTest::removeEntryFromCategory_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.removeEntryFromCategory(1, 1));
}

/**
 * @brief Test removePersona when the persona exists.
 */
void VaultControllerTest::removePersona_returnsTrue_whenPersonaExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t firstPersonaId = 0;
    int64_t secondPersonaId = 0;
    controller.session = makeVaultSessionWithPersonas(&firstPersonaId, &secondPersonaId);

    QVERIFY(controller.removePersona(firstPersonaId));
    QCOMPARE(controller.session->getPersonas().size(), std::size_t(1));
    QCOMPARE(controller.session->getPersonas().front()->getId(), secondPersonaId);
}

/**
 * @brief Test removePersona when the persona does not exist.
 */
void VaultControllerTest::removePersona_returnsFalse_whenPersonaDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t firstPersonaId = 0;
    int64_t secondPersonaId = 0;
    controller.session = makeVaultSessionWithPersonas(&firstPersonaId, &secondPersonaId);

    QVERIFY(!controller.removePersona(secondPersonaId + 1));
    QCOMPARE(controller.session->getPersonas().size(), std::size_t(2));
    QCOMPARE(controller.session->getPersonas()[0]->getId(), firstPersonaId);
    QCOMPARE(controller.session->getPersonas()[1]->getId(), secondPersonaId);
}

/**
 * @brief Test removePersona when no session is open.
 */
void VaultControllerTest::removePersona_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.removePersona(1));
}

/**
 * @brief Test setAliasForWebsite when the website exists.
 */
void VaultControllerTest::setAliasForWebsite_returnsTrue_whenWebsiteExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t websiteId = 0;
    controller.session = makeVaultSessionWithWebsite(&categoryId, &websiteId);

    QVERIFY(controller.setAliasForWebsite(categoryId, websiteId, "example-alias-id", "Example alias"));

    const auto *updatedWebsite = controller.session->getWebsiteById(websiteId);
    QVERIFY(updatedWebsite != nullptr);
    QCOMPARE(updatedWebsite->getAliasId(), std::string("example-alias-id"));
    QCOMPARE(updatedWebsite->getAlias(), std::string("Example alias"));
}

/**
 * @brief Test setAliasForWebsite when the category does not exist.
 */
void VaultControllerTest::setAliasForWebsite_returnsFalse_whenCategoryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t websiteId = 0;
    controller.session = makeVaultSessionWithWebsite(&categoryId, &websiteId);

    QVERIFY(!controller.setAliasForWebsite(categoryId + 1, websiteId, "example-alias-id", "Example alias"));

    const auto *storedWebsite = controller.session->getWebsiteById(websiteId);
    QVERIFY(storedWebsite != nullptr);
    QCOMPARE(storedWebsite->getAliasId(), std::string(""));
    QCOMPARE(storedWebsite->getAlias(), std::string(""));
}

/**
 * @brief Test setAliasForWebsite when the entry does not exist.
 */
void VaultControllerTest::setAliasForWebsite_returnsFalse_whenEntryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t websiteId = 0;
    controller.session = makeVaultSessionWithWebsite(&categoryId, &websiteId);

    QVERIFY(!controller.setAliasForWebsite(categoryId, websiteId + 1, "example-alias-id", "Example alias"));

    const auto *storedWebsite = controller.session->getWebsiteById(websiteId);
    QVERIFY(storedWebsite != nullptr);
    QCOMPARE(storedWebsite->getAliasId(), std::string(""));
    QCOMPARE(storedWebsite->getAlias(), std::string(""));
}

/**
 * @brief Test setAliasForWebsite when the targeted entry has a wrong type.
 */
void VaultControllerTest::setAliasForWebsite_returnsFalse_whenEntryHasWrongType()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    int64_t categoryId = 0;
    int64_t websiteId = 0;
    int64_t wrongEntryId = 0;
    controller.session = makeVaultSessionWithWebsite(&categoryId, &websiteId, &wrongEntryId);

    QVERIFY(!controller.setAliasForWebsite(categoryId, wrongEntryId, "example-alias-id", "Example alias"));

    const auto *storedWebsite = controller.session->getWebsiteById(websiteId);
    QVERIFY(storedWebsite != nullptr);
    QCOMPARE(storedWebsite->getAliasId(), std::string(""));
    QCOMPARE(storedWebsite->getAlias(), std::string(""));
}

/**
 * @brief Test setAliasForWebsite when no session is open.
 */
void VaultControllerTest::setAliasForWebsite_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController &controller = VaultController::getInstance("vault", std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             controller.setAliasForWebsite(1, 1, "alias-id", "Alias"));
}

QTEST_MAIN(VaultControllerTest)

#include "tst_vaultcontroller.moc"
