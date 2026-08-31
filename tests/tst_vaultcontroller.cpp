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

QTEST_MAIN(VaultControllerTest)

#include "tst_vaultcontroller.moc"

