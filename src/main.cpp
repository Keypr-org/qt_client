#include "appconfig.h"
#include "mainwindow.h"
#include "vaultcontroller.h"
#include "vaultstoragesetupdialog.h"

#include <QApplication>
#include <QFont>

namespace {
void applyPlatformFont()
{
#if defined(Q_OS_WIN)
    QFont font;
    font.setFamilies({"Segoe UI", "Segoe UI Variable Text", "Tahoma", "sans-serif"});
    QApplication::setFont(font);
#elif defined(Q_OS_LINUX)
    QFont font;
    font.setFamilies({"Noto Sans", "Cantarell", "Ubuntu", "DejaVu Sans", "sans-serif"});
    QApplication::setFont(font);
#endif
}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Keypr");
    QApplication::setApplicationDisplayName("Keypr");

    applyPlatformFont();

    AppConfig config = AppConfig::load();
    if (!config.hasValidVaultStoragePath()) {
        VaultStorageSetupDialog setupDialog;
        setupDialog.exec();
    }

    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.showMaximized();

    QObject::connect(&a, &QApplication::aboutToQuit, [&]()
    {
        auto &vaultController = VaultController::getInstance();
        if (vaultController.isVaultUnlocked()) {
            vaultController.lockVault();
        }
    });

    return QApplication::exec();
}
