#include <QtTest/QtTest>
#include <memory>
#include "fakevaultrepository.h"
#include "../src/vaultcontroller.h"

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

QTEST_MAIN(VaultControllerTest)

#include "tst_vaultcontroller.moc"

