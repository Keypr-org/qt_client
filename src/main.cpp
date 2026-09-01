#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Keypr");
    QApplication::setApplicationDisplayName("Keypr");

    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.showMaximized();

    return QApplication::exec();
}
