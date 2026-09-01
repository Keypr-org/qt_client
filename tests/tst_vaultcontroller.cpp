#include <QtTest/QtTest>
#include <array>
#include <chrono>
#include <memory>
#include "fakevaultrepository.h"
#include "fakevaultsession.h"
#define private public
#include "../src/vaultcontroller.h"
#undef private

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

class VaultControllerTest : public QObject
{
    Q_OBJECT

private slots:
    void openVault_returnsTrue_whenFileExistsAndParsingSucceeds();
    void openVault_returnsFalse_whenFileExistsAndParsingFails();
    void openVault_returnsFalse_whenFileDoesNotExistAndParsingSucceeds();
    void openVault_returnsFalse_whenFileDoesNotExistAndParsingFails();
    void unlockVault_returnsTrue_whenEverythingIsCorrect();
    void unlockVault_returnsFalse_whenPasswordIsWrongButParsingSucceeds();
    void unlockVault_returnsFalse_whenEverythingFailsAndParsingFails();
    void createVault_returnsTrue_whenVaultCreationSucceeds();
    void createVault_returnsFalse_whenVaultCreationFails();
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
};

/**
 * @brief Test the openVault method when the file exists and parsing succeeds.
 */
void VaultControllerTest::openVault_returnsTrue_whenFileExistsAndParsingSucceeds()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    QVERIFY(controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file exists and parsing fails.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileExistsAndParsingFails()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, false);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file does not exist and parsing succeeds.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileDoesNotExistAndParsingSucceeds()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, true);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file does not exist and parsing fails.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileDoesNotExistAndParsingFails()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, false);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the unlockVault method when everything is correct.
 */
void VaultControllerTest::unlockVault_returnsTrue_whenEverythingIsCorrect()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    QVERIFY(controller.unlockVault("master-password", "vault"));
    QVERIFY(controller.isVaultUnlocked());
}

/**
 * @brief Test the unlockVault method when the password is wrong but parsing succeeds.
 */
void VaultControllerTest::unlockVault_returnsFalse_whenPasswordIsWrongButParsingSucceeds()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, true);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.unlockVault("wrong-password", "vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test the unlockVault method when everything fails and parsing fails.
 */
void VaultControllerTest::unlockVault_returnsFalse_whenEverythingFailsAndParsingFails()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, false);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.unlockVault("wrong-password", "vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test the createVault method when vault creation succeeds.
 */
void VaultControllerTest::createVault_returnsTrue_whenVaultCreationSucceeds()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    QVERIFY(controller.createVault("master-password", "vault"));
    QVERIFY(controller.isVaultUnlocked());
}

/**
 * @brief Test the createVault method when vault creation fails.
 */
void VaultControllerTest::createVault_returnsFalse_whenVaultCreationFails()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, false);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.createVault("master-password", "vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test linkPersonaToEntry when the persona and entry both exist.
 */
void VaultControllerTest::linkPersonaToEntry_returnsTrue_whenPersonaAndEntryExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY(controller.linkPersonaToEntry(personaId, categoryId, targetWebsiteId));

    const auto &entries = controller.getCategories().front()->getEntries();
    const auto *targetWebsite = dynamic_cast<const Website *>(entries[0].get());
    const auto *untouchedWebsite = dynamic_cast<const Website *>(entries[1].get());

    QVERIFY(targetWebsite != nullptr);
    QVERIFY(untouchedWebsite != nullptr);
    QCOMPARE(targetWebsite->getId(), targetWebsiteId);
    QCOMPARE(targetWebsite->getPersonaId(), personaId);
    QCOMPARE(untouchedWebsite->getId(), untouchedWebsiteId);
    QCOMPARE(untouchedWebsite->getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when the persona does not exist.
 */
void VaultControllerTest::linkPersonaToEntry_returnsFalse_whenPersonaDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY(!controller.linkPersonaToEntry(personaId + 1, categoryId, targetWebsiteId));

    const auto *targetWebsite = dynamic_cast<const Website *>(
        controller.getCategories().front()->getEntries()[0].get());
    QVERIFY(targetWebsite != nullptr);
    QCOMPARE(targetWebsite->getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when the category does not exist.
 */
void VaultControllerTest::linkPersonaToEntry_returnsFalse_whenCategoryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY(!controller.linkPersonaToEntry(personaId, categoryId + 1, targetWebsiteId));

    const auto *targetWebsite = dynamic_cast<const Website *>(
        controller.getCategories().front()->getEntries()[0].get());
    QVERIFY(targetWebsite != nullptr);
    QCOMPARE(targetWebsite->getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when the entry does not exist.
 */
void VaultControllerTest::linkPersonaToEntry_returnsFalse_whenEntryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t targetWebsiteId = 0;
    int64_t untouchedWebsiteId = 0;
    controller.session = makeLinkedPersonaSession(&personaId, &categoryId, &targetWebsiteId,
                                                  &untouchedWebsiteId);

    QVERIFY(!controller.linkPersonaToEntry(personaId, categoryId, -1));

    const auto *targetWebsite = dynamic_cast<const Website *>(
        controller.getCategories().front()->getEntries()[0].get());
    QVERIFY(targetWebsite != nullptr);
    QCOMPARE(targetWebsite->getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when the targeted entry has a wrong type.
 */
void VaultControllerTest::linkPersonaToEntry_returnsFalse_whenEntryHasWrongType()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    int64_t personaId = 0;
    int64_t categoryId = 0;
    int64_t websiteId = 0;
    int64_t wrongEntryId = 0;
    controller.session = makeLinkedPersonaSessionWithWrongEntry(&personaId, &categoryId,
                                                                &websiteId, &wrongEntryId);

    QVERIFY(!controller.linkPersonaToEntry(personaId, categoryId, wrongEntryId));

    const auto *website = dynamic_cast<const Website *>(controller.getCategories().front()->getEntries()[0].get());
    QVERIFY(website != nullptr);
    QCOMPARE(website->getId(), websiteId);
    QCOMPARE(website->getPersonaId(), static_cast<int64_t>(NO_PERSONA_ID));
}

/**
 * @brief Test linkPersonaToEntry when no session is open.
 */
void VaultControllerTest::linkPersonaToEntry_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.linkPersonaToEntry(1, 1, 1));
}

/**
 * @brief Test getCategories when the session is open and contains categories.
 */
void VaultControllerTest::getCategories_returnsCategories_whenSessionIsOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));
    controller.session = makeVaultSession();
    controller.session->addCategory(std::make_unique<Category>("Passwords"));
    controller.session->addCategory(std::make_unique<Category>("Websites"));

    const auto &categories = controller.getCategories();

    QCOMPARE(categories.size(), std::size_t(2));
    QCOMPARE(QString::fromStdString(categories[0]->getName()), QString("Passwords"));
    QCOMPARE(QString::fromStdString(categories[1]->getName()), QString("Websites"));
}

/**
 * @brief Test getCategories when the session is open but has no categories.
 */
void VaultControllerTest::getCategories_returnsEmptyVector_whenSessionIsOpenButEmpty()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));
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
    VaultController controller(std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.getCategories());
}

/**
 * @brief Test removeEntryFromCategory when the entry exists in the category.
 */
void VaultControllerTest::removeEntryFromCategory_returnsTrue_whenEntryExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    QVERIFY(controller.removeEntryFromCategory(categoryId, entryId));
    QCOMPARE(controller.getCategories().front()->getEntries().size(), std::size_t(0));
}

/**
 * @brief Test removeEntryFromCategory when the category does not exist.
 */
void VaultControllerTest::removeEntryFromCategory_returnsFalse_whenCategoryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    QVERIFY(!controller.removeEntryFromCategory(categoryId + 1, entryId));
    QCOMPARE(controller.getCategories().front()->getEntries().size(), std::size_t(1));
}

/**
 * @brief Test removeEntryFromCategory when the entry does not exist in the category.
 */
void VaultControllerTest::removeEntryFromCategory_returnsFalse_whenEntryDoesNotExist()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    int64_t categoryId = 0;
    int64_t entryId = 0;
    controller.session = makeVaultSessionWithCategoryAndEntry(&categoryId, &entryId);

    QVERIFY(!controller.removeEntryFromCategory(categoryId, entryId + 1));
    QCOMPARE(controller.getCategories().front()->getEntries().size(), std::size_t(1));
}

/**
 * @brief Test removeEntryFromCategory when no session is open.
 */
void VaultControllerTest::removeEntryFromCategory_throws_whenSessionIsNotOpen()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.removeEntryFromCategory(1, 1));
}

/**
 * @brief Test removePersona when the persona exists.
 */
void VaultControllerTest::removePersona_returnsTrue_whenPersonaExists()
{
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

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
    VaultController controller(std::move(repository));

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
    VaultController controller(std::move(repository));

    QVERIFY_THROWS_EXCEPTION(std::runtime_error, controller.removePersona(1));
}

QTEST_MAIN(VaultControllerTest)

#include "tst_vaultcontroller.moc"
