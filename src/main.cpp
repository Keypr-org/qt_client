#include "formOverlay/createvaultoverlay.h"
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.show();

    /*CreateVaultOverlay u;
    u.setLabelText("Maman");
    u.setInputPlaceholder("I'm a placeholder");
    u.show();*/
    return QApplication::exec();
}
