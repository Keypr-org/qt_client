#include <QtTest/QtTest>
#include <memory>
#include "fakevaultrepository.h"
#include "fakevaultsession.h"
#define private public
#include "../src/vaultcontroller.h"
#undef private

static std::unique_ptr<VaultSession> makeVaultSession() {
    auto vaultSession = std::make_unique<FakeVaultSession>();
    return vaultSession;
}

class VaultControllerTest : public QObject {
    Q_OBJECT

private slots:
    void openVault_returnsTrue_whenFileExistsAndParsingSucceeds();
    void openVault_returnsFalse_whenFileExistsAndParsingFails();
    void openVault_returnsFalse_whenFileDoesNotExistAndParsingSucceeds();
    void openVault_returnsFalse_whenFileDoesNotExistAndParsingFails();
    void unlockVault_returnsTrue_whenEverythingIsCorrect();
    void unlockVault_returnsFalse_whenPasswordIsWrongButParsingSucceeds();
    void unlockVault_returnsFalse_whenEverythingFailsAndParsingFails();
    void getCategories_returnsCategories_whenSessionIsOpen();
    void getCategories_returnsEmptyVector_whenSessionIsOpenButEmpty();
    void getCategories_throws_whenSessionIsNotOpen();
};

/**
 * @brief Test the openVault method when the file exists and parsing succeeds.
 */
void VaultControllerTest::openVault_returnsTrue_whenFileExistsAndParsingSucceeds() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    QVERIFY(controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file exists and parsing fails.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileExistsAndParsingFails() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, false);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file does not exist and parsing succeeds.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileDoesNotExistAndParsingSucceeds() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, true);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file does not exist and parsing fails.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileDoesNotExistAndParsingFails() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, false);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the unlockVault method when everything is correct.
 */
void VaultControllerTest::unlockVault_returnsTrue_whenEverythingIsCorrect() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    QVERIFY(controller.unlockVault("master-password", "vault"));
    QVERIFY(controller.isVaultUnlocked());
}

/**
 * @brief Test the unlockVault method when the password is wrong but parsing succeeds.
 */
void VaultControllerTest::unlockVault_returnsFalse_whenPasswordIsWrongButParsingSucceeds() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, true);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.unlockVault("wrong-password", "vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test the unlockVault method when everything fails and parsing fails.
 */
void VaultControllerTest::unlockVault_returnsFalse_whenEverythingFailsAndParsingFails() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(false, false);
    VaultController controller(std::move(repository));

    QVERIFY(!controller.unlockVault("wrong-password", "vault"));
    QVERIFY(!controller.isVaultUnlocked());
}

/**
 * @brief Test getCategories when the session is open and contains categories.
 */
void VaultControllerTest::getCategories_returnsCategories_whenSessionIsOpen() {
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
void VaultControllerTest::getCategories_returnsEmptyVector_whenSessionIsOpenButEmpty() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));
    controller.session = makeVaultSession();

    const auto &categories = controller.getCategories();

    QCOMPARE(categories.size(), std::size_t(0));
}

/**
 * @brief Test getCategories when no session is open.
 */
void VaultControllerTest::getCategories_throws_whenSessionIsNotOpen() {
    std::unique_ptr<FakeVaultRepository> repository = std::make_unique<FakeVaultRepository>(true, true);
    VaultController controller(std::move(repository));

    QVERIFY_EXCEPTION_THROWN(controller.getCategories(), std::runtime_error);
}

QTEST_MAIN(VaultControllerTest)

#include "tst_vaultcontroller.moc"

