#include "appconfig.h"
#include "mainwindow.h"
#include "vaultstoragesetupdialog.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Keypr");
    QApplication::setApplicationDisplayName("Keypr");

    AppConfig config = AppConfig::load();
    if (!config.hasValidVaultStoragePath()) {
        VaultStorageSetupDialog setupDialog;
        setupDialog.exec();
    }

    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.showMaximized();

    return QApplication::exec();
}
