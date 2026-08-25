#include "mainContent/viewentries.h"
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.show();*/

    ViewEntries u;
    u.show();
    return QApplication::exec();
}
