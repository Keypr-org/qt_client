#include <QtTest/QtTest>

#include "fakevaultrepository.h"
#include "../src/vaultcontroller.h"

class VaultControllerTest : public QObject {
    Q_OBJECT

private slots:
    void openVault_returnsTrue_whenFileExistsAndParsingSucceeds();
    void openVault_returnsFalse_whenFileExistsAndParsingFails();
    void openVault_returnsFalse_whenFileDoesNotExistAndParsingSucceeds();
    void openVault_returnsFalse_whenFileDoesNotExistAndParsingFails();
};

/**
 * @brief Test the openVault method when the file exists and parsing succeeds.
 */
void VaultControllerTest::openVault_returnsTrue_whenFileExistsAndParsingSucceeds() {
    FakeVaultRepository repository(true, true);
    VaultController controller(repository);

    QVERIFY(controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file exists and parsing fails.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileExistsAndParsingFails() {
    FakeVaultRepository repository(true, false);
    VaultController controller(repository);

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file does not exist and parsing succeeds.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileDoesNotExistAndParsingSucceeds() {
    FakeVaultRepository repository(false, true);
    VaultController controller(repository);

    QVERIFY(!controller.vaultExists("vault"));
}

/**
 * @brief Test the openVault method when the file does not exist and parsing fails.
 */
void VaultControllerTest::openVault_returnsFalse_whenFileDoesNotExistAndParsingFails() {
    FakeVaultRepository repository(false, false);
    VaultController controller(repository);

    QVERIFY(!controller.vaultExists("vault"));
}

QTEST_MAIN(VaultControllerTest)

#include "tst_vaultcontroller.moc"

