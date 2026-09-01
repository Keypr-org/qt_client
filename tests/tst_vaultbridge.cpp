#include <QtTest/QtTest>
#include <QDate>
#include <QTemporaryDir>
#include <algorithm>

#define private public
#include "../src/vaultbridge.h"
#undef private

#include "VaultRepository.h"
#include "vaultcontroller.h"
#include "entities/Category.h"
#include "entities/CreditCard.h"
#include "entities/Website.h"
#include "entities/Wifi.h"

class VaultBridgeTest : public QObject {
    Q_OBJECT

private slots:
    void createVault_writesRealEncryptedVaultFile_underVaultStoragePath();
    void createVault_returnsFalse_whenVaultStoragePathDoesNotExist();
    void unlockVault_returnsSuccess_whenPasswordIsCorrect();
    void unlockVault_returnsIncorrectPasswordOrCorrupted_whenPasswordIsWrong();
    void unlockVault_returnsVaultFileNotFound_whenNoSuchVault();
    void lockVault_savesChangesToDiskAndRelocks();
    void lockVault_returnsFalse_whenNoVaultUnlocked();
    void addCategory_andCategories_roundTripThroughRealSession();
    void categories_returnsEmptyList_whenNoVaultUnlocked();
    void addCategory_returnsFalse_whenNoVaultUnlocked();
    void entriesInCategory_returnsRealEntriesOfEachKind();
    void entriesInCategory_returnsEmptyList_whenNoVaultUnlocked();
    void addRemoveEntries_roundTripThroughRealSession();
    void updateEntries_mutateRealSessionInPlace();
    void personaLinking_roundTripsThroughRealWebsiteEntry();
    void linkPersonaToWebsite_returnsFalse_whenPersonaDoesNotExist();
    void personas_addUpdateRemove_roundTripThroughRealSession();
    void searchEntriesInCategory_matchesContentAcrossKinds();
    void searchEntriesInCategory_emptyTermMatchesEverything();
    void searchEntriesInCategory_returnsEmptyList_whenCategoryDoesNotExist();
};

/**
 * @brief createVault() should go through VaultController::createVault()/lockVault() and leave a
 * genuine, parseable encrypted vault file at the predicted path.
 */
void VaultBridgeTest::createVault_writesRealEncryptedVaultFile_underVaultStoragePath() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    const QString expectedPath = bridge.vaultFilePath("My Vault");

    QVERIFY(!QFileInfo::exists(expectedPath));

    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));

    QVERIFY(QFileInfo::exists(expectedPath));
    QVERIFY(QFileInfo(expectedPath).size() > 0);

    // Prove it's a real, parseable vault file (as produced by VaultRepository::lockVault()),
    // not just an arbitrary file dropped at that path.
    VaultRepository repository;
    QVERIFY(repository.vaultExists(expectedPath.toStdString()));
}

/**
 * @brief createVault() should fail (rather than crash or silently succeed) when the configured
 * vault storage path does not exist on disk.
 */
void VaultBridgeTest::createVault_returnsFalse_whenVaultStoragePathDoesNotExist() {
    VaultBridge bridge("/this/path/does/not/exist");

    QVERIFY(!bridge.createVault("My Vault", "correct-horse-battery-staple"));
}

/**
 * @brief unlockVault() should succeed via VaultController::unlockVault() when given the exact
 * master password a vault was created with.
 */
void VaultBridgeTest::unlockVault_returnsSuccess_whenPasswordIsCorrect() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));

    QCOMPARE(bridge.unlockVault("My Vault", "correct-horse-battery-staple"), VaultBridge::UnlockResult::Success);
}

/**
 * @brief unlockVault() should not unlock, and should report the failure distinctly, when the
 * password is wrong.
 */
void VaultBridgeTest::unlockVault_returnsIncorrectPasswordOrCorrupted_whenPasswordIsWrong() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));

    QCOMPARE(bridge.unlockVault("My Vault", "wrong-password"), VaultBridge::UnlockResult::IncorrectPasswordOrCorrupted);
}

/**
 * @brief unlockVault() should report a missing vault file distinctly from a wrong password.
 */
void VaultBridgeTest::unlockVault_returnsVaultFileNotFound_whenNoSuchVault() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());

    QCOMPARE(bridge.unlockVault("Never Created", "whatever"), VaultBridge::UnlockResult::VaultFileNotFound);
}

/**
 * @brief lockVault() should persist changes made on the unlocked session to disk (see
 * VaultController::lockVault()) and lock it, so a fresh unlock sees those changes and the
 * session can no longer be used until unlocked again.
 */
void VaultBridgeTest::lockVault_savesChangesToDiskAndRelocks() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    QVERIFY(bridge.addCategory("Passwords"));

    QVERIFY(bridge.lockVault());

    // Session is locked: mutating operations should no longer report success.
    QVERIFY(!bridge.addCategory("Wifi"));

    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    const auto categories = bridge.categories();
    QCOMPARE(categories.size(), 1);
    QCOMPARE(categories.first().name, QString("Passwords"));
}

/**
 * @brief lockVault() should fail rather than throw when no vault is currently unlocked.
 */
void VaultBridgeTest::lockVault_returnsFalse_whenNoVaultUnlocked() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());

    QVERIFY(!bridge.lockVault());
}

/**
 * @brief Categories added via addCategory() on an unlocked (just-created) vault should show up
 * in categories(), in the real VaultController session (see VaultController::addCategory() and
 * VaultController::getCategories()).
 */
void VaultBridgeTest::addCategory_andCategories_roundTripThroughRealSession() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);

    QVERIFY(bridge.categories().isEmpty());

    QVERIFY(bridge.addCategory("Passwords"));
    QVERIFY(bridge.addCategory("Wifi"));

    const auto categories = bridge.categories();
    QCOMPARE(categories.size(), 2);
    QCOMPARE(categories[0].name, QString("Passwords"));
    QCOMPARE(categories[1].name, QString("Wifi"));
    QVERIFY(categories[0].id != categories[1].id);
}

/**
 * @brief categories() should return an empty list rather than throwing when no vault is
 * currently unlocked.
 */
void VaultBridgeTest::categories_returnsEmptyList_whenNoVaultUnlocked() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());

    QVERIFY(bridge.categories().isEmpty());
}

/**
 * @brief addCategory() should fail rather than throw when no vault is currently unlocked.
 */
void VaultBridgeTest::addCategory_returnsFalse_whenNoVaultUnlocked() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());

    QVERIFY(!bridge.addCategory("Passwords"));
}

/**
 * @brief entriesInCategory() should reflect real Website/Wifi/CreditCard entries added directly
 * through the underlying VaultController session (see VaultController::addEntryToCategory() and
 * VaultController::getEntriesInCategory()), converting each into the matching EntrySummary kind
 * with its fields.
 */
void VaultBridgeTest::entriesInCategory_returnsRealEntriesOfEachKind() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    QVERIFY(bridge.addCategory("Everything"));
    const qint64 categoryId = bridge.categories().first().id;

    bridge.m_controller->addEntryToCategory(categoryId,
        std::make_unique<Website>("web notes", "GitHub", "octocat", "hunter2",
                                   "https://github.com", "some comments"));
    bridge.m_controller->addEntryToCategory(categoryId,
        std::make_unique<Wifi>("Sunrise", "sunrise-password", "wifi notes"));
    bridge.m_controller->addEntryToCategory(categoryId,
        std::make_unique<CreditCard>("Alex Morgan", "4111111111111111", "12/27", "123", "card notes"));

    const auto entries = bridge.entriesInCategory(categoryId);
    QCOMPARE(entries.size(), 3);

    const auto website = std::find_if(entries.begin(), entries.end(), [](const auto &e) {
        return e.kind == VaultBridge::EntryKind::Website;
    });
    QVERIFY(website != entries.end());
    QCOMPARE(website->title, QString("GitHub"));
    QCOMPARE(website->username, QString("octocat"));
    QCOMPARE(website->password, QString("hunter2"));
    QCOMPARE(website->url, QString("https://github.com"));
    QCOMPARE(website->comments, QString("some comments"));
    QCOMPARE(website->notes, QString("web notes"));

    const auto wifi = std::find_if(entries.begin(), entries.end(), [](const auto &e) {
        return e.kind == VaultBridge::EntryKind::Wifi;
    });
    QVERIFY(wifi != entries.end());
    QCOMPARE(wifi->networkName, QString("Sunrise"));
    QCOMPARE(wifi->password, QString("sunrise-password"));
    QCOMPARE(wifi->notes, QString("wifi notes"));

    const auto card = std::find_if(entries.begin(), entries.end(), [](const auto &e) {
        return e.kind == VaultBridge::EntryKind::CreditCard;
    });
    QVERIFY(card != entries.end());
    QCOMPARE(card->cardHolderName, QString("Alex Morgan"));
    QCOMPARE(card->cardNumber, QString("4111111111111111"));
    QCOMPARE(card->expiration, QString("12/27"));
    QCOMPARE(card->securityCode, QString("123"));
    QCOMPARE(card->notes, QString("card notes"));
}

/**
 * @brief entriesInCategory() should return an empty list rather than throwing when no vault is
 * currently unlocked.
 */
void VaultBridgeTest::entriesInCategory_returnsEmptyList_whenNoVaultUnlocked() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());

    QVERIFY(bridge.entriesInCategory(1).isEmpty());
}

/**
 * @brief addWebsiteEntry()/addWifiEntry()/addCreditCardEntry() should create real entries
 * visible via entriesInCategory(), and removeEntry() should make them disappear again (see
 * VaultController::addEntryToCategory()/removeEntryFromCategory()).
 */
void VaultBridgeTest::addRemoveEntries_roundTripThroughRealSession() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    QVERIFY(bridge.addCategory("Everything"));
    const qint64 categoryId = bridge.categories().first().id;

    const QString websiteId = bridge.addWebsiteEntry(categoryId, "GitHub", "octocat", "hunter2", "https://github.com", "comments", "notes");
    const QString wifiId = bridge.addWifiEntry(categoryId, "Sunrise", "sunrise-password", "wifi notes");
    const QString cardId = bridge.addCreditCardEntry(categoryId, "Alex Morgan", "4111111111111111", "12/27", "123", "card notes");

    QVERIFY(!websiteId.isEmpty());
    QVERIFY(!wifiId.isEmpty());
    QVERIFY(!cardId.isEmpty());
    QCOMPARE(bridge.entriesInCategory(categoryId).size(), 3);

    QVERIFY(bridge.removeEntry(categoryId, wifiId));
    QCOMPARE(bridge.entriesInCategory(categoryId).size(), 2);
    QVERIFY(!bridge.removeEntry(categoryId, wifiId));
}

/**
 * @brief updateWebsiteEntry()/updateWifiEntry()/updateCreditCardEntry() should mutate the real
 * entry object in place (via Category::findEntryById(), see the findEntry() helper in
 * vaultbridge.cpp), with the change visible on the next entriesInCategory() read.
 */
void VaultBridgeTest::updateEntries_mutateRealSessionInPlace() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    QVERIFY(bridge.addCategory("Everything"));
    const qint64 categoryId = bridge.categories().first().id;

    const QString websiteId = bridge.addWebsiteEntry(categoryId, "GitHub", "octocat", "hunter2", "https://github.com", "comments", "notes");
    QVERIFY(bridge.updateWebsiteEntry(websiteId, "new-user", "new-pass", "https://new.example.com", "new comments", "new notes"));

    const auto entries = bridge.entriesInCategory(categoryId);
    const auto website = std::find_if(entries.begin(), entries.end(), [&](const auto &e) { return e.id == websiteId; });
    QVERIFY(website != entries.end());
    QCOMPARE(website->username, QString("new-user"));
    QCOMPARE(website->password, QString("new-pass"));
    QCOMPARE(website->url, QString("https://new.example.com"));
    QCOMPARE(website->comments, QString("new comments"));
    QCOMPARE(website->notes, QString("new notes"));

    QVERIFY(!bridge.updateWebsiteEntry("not-a-real-id", "a", "b", "c", "d", "e"));
}

/**
 * @brief linkPersonaToWebsite()/unlinkPersonaFromWebsite() should mutate the real Website's
 * personaId in place, visible via EntrySummary::personaId on the next read.
 */
void VaultBridgeTest::personaLinking_roundTripsThroughRealWebsiteEntry() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    QVERIFY(bridge.addCategory("Everything"));
    const qint64 categoryId = bridge.categories().first().id;

    const QString websiteId = bridge.addWebsiteEntry(categoryId, "GitHub", "octocat", "hunter2", "https://github.com", "", "");
    const qint64 personaId = bridge.addPersona("Ada", "Lovelace", QDate(1990, 1, 1), "Address", "555-1234");
    QVERIFY(personaId >= 0);

    QCOMPARE(bridge.entriesInCategory(categoryId).first().personaId, qint64(-1));

    QVERIFY(bridge.linkPersonaToWebsite(categoryId, websiteId, personaId));
    QCOMPARE(bridge.entriesInCategory(categoryId).first().personaId, personaId);

    QVERIFY(bridge.unlinkPersonaFromWebsite(websiteId));
    QCOMPARE(bridge.entriesInCategory(categoryId).first().personaId, qint64(-1));

    QVERIFY(bridge.setWebsiteAlias(websiteId, "alias-id", "alias@example.com"));
    QCOMPARE(bridge.entriesInCategory(categoryId).first().aliasId, QString("alias-id"));
    QCOMPARE(bridge.entriesInCategory(categoryId).first().alias, QString("alias@example.com"));
}

/**
 * @brief linkPersonaToWebsite() goes through VaultController::linkPersonaToEntry(), which
 * validates the persona actually exists before linking — unlike a blind field mutation, it must
 * fail for a bogus persona id rather than silently storing it.
 */
void VaultBridgeTest::linkPersonaToWebsite_returnsFalse_whenPersonaDoesNotExist() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    QVERIFY(bridge.addCategory("Everything"));
    const qint64 categoryId = bridge.categories().first().id;

    const QString websiteId = bridge.addWebsiteEntry(categoryId, "GitHub", "octocat", "hunter2", "https://github.com", "", "");

    QVERIFY(!bridge.linkPersonaToWebsite(categoryId, websiteId, 999999));
    QCOMPARE(bridge.entriesInCategory(categoryId).first().personaId, qint64(-1));
}

/**
 * @brief addPersona()/updatePersona()/removePersona() should round-trip through the real
 * VaultController session (see VaultController::addPersona()/getPersonaById()/removePersona()).
 */
void VaultBridgeTest::personas_addUpdateRemove_roundTripThroughRealSession() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);

    QVERIFY(bridge.personas().isEmpty());

    const QDate birthday(1815, 12, 10);
    const qint64 id = bridge.addPersona("Ada", "Lovelace", birthday, "London", "555-1234");
    QVERIFY(id >= 0);

    auto personas = bridge.personas();
    QCOMPARE(personas.size(), 1);
    QCOMPARE(personas.first().firstName, QString("Ada"));
    QCOMPARE(personas.first().lastName, QString("Lovelace"));
    QCOMPARE(personas.first().dateOfBirth, birthday);
    QCOMPARE(personas.first().address, QString("London"));
    QCOMPARE(personas.first().phone, QString("555-1234"));

    QVERIFY(bridge.updatePersona(id, "Augusta", "King", birthday, "Surrey", "555-6789"));
    personas = bridge.personas();
    QCOMPARE(personas.first().firstName, QString("Augusta"));
    QCOMPARE(personas.first().lastName, QString("King"));
    QCOMPARE(personas.first().address, QString("Surrey"));
    QCOMPARE(personas.first().phone, QString("555-6789"));

    QVERIFY(!bridge.updatePersona(999999, "X", "Y", birthday, "Z", "W"));

    QVERIFY(bridge.removePersona(id));
    QVERIFY(bridge.personas().isEmpty());
    QVERIFY(!bridge.removePersona(id));
}

/**
 * @brief searchEntriesInCategory() should match on content fields across every entry kind (see
 * VaultController::searchEntriesInCategory() / VaultSession::searchEntriesInCategory()): notes
 * for any kind, plus title/username/url/comments/alias for websites, network name for wifi, and
 * cardholder name for credit cards.
 */
void VaultBridgeTest::searchEntriesInCategory_matchesContentAcrossKinds() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    QVERIFY(bridge.addCategory("Everything"));
    const qint64 categoryId = bridge.categories().first().id;

    const QString websiteId = bridge.addWebsiteEntry(categoryId, "GitHub", "octocat", "hunter2", "https://github.com", "comments", "");
    bridge.addWifiEntry(categoryId, "Sunrise", "sunrise-password", "");
    bridge.addCreditCardEntry(categoryId, "Alex Morgan", "4111111111111111", "12/27", "123", "");

    const auto githubMatches = bridge.searchEntriesInCategory(categoryId, "GitHub");
    QCOMPARE(githubMatches.size(), 1);
    QCOMPARE(githubMatches.first().id, websiteId);

    QCOMPARE(bridge.searchEntriesInCategory(categoryId, "Sunrise").size(), 1);
    QCOMPARE(bridge.searchEntriesInCategory(categoryId, "Morgan").size(), 1);
    QVERIFY(bridge.searchEntriesInCategory(categoryId, "no-such-term-anywhere").isEmpty());
}

/**
 * @brief An empty search term should match every entry in the category (see
 * VaultSession::searchEntriesInCategory(), which uses std::string::find("") == 0 for every
 * entry's notes).
 */
void VaultBridgeTest::searchEntriesInCategory_emptyTermMatchesEverything() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);
    QVERIFY(bridge.addCategory("Everything"));
    const qint64 categoryId = bridge.categories().first().id;

    bridge.addWebsiteEntry(categoryId, "GitHub", "octocat", "hunter2", "https://github.com", "", "");
    bridge.addWifiEntry(categoryId, "Sunrise", "sunrise-password", "");

    QCOMPARE(bridge.searchEntriesInCategory(categoryId, "").size(), 2);
}

/**
 * @brief searchEntriesInCategory() should return an empty list rather than throwing when the
 * category doesn't exist (VaultController::searchEntriesInCategory() throws in that case, unlike
 * getEntriesInCategory()).
 */
void VaultBridgeTest::searchEntriesInCategory_returnsEmptyList_whenCategoryDoesNotExist() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    VaultBridge bridge(tempDir.path());
    QVERIFY(bridge.createVault("My Vault", "correct-horse-battery-staple"));
    QVERIFY(bridge.unlockVault("My Vault", "correct-horse-battery-staple") == VaultBridge::UnlockResult::Success);

    QVERIFY(bridge.searchEntriesInCategory(999999, "anything").isEmpty());
}

QTEST_MAIN(VaultBridgeTest)

#include "tst_vaultbridge.moc"
