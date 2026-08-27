#include "mainContent/entries/wifientry.h"
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.show();*/

    WifiEntry u;
    u.show();
    return QApplication::exec();
}
