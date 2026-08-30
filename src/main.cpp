#include "mainwindow.h"
#include "mainContent/newpersonaform.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.showMaximized();

    return QApplication::exec();
}
