#include "mainwindow.h"

#include <QApplication>

#include "mainContent/unlockvaultmodal.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.setWindowTitle("Keypr | Your local password manager");
    //w.show();

    unlockvaultmodal u;
    u.show();
    return QApplication::exec();
}
