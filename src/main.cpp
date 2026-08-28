#include "mainwindow.h"
#include "mainContent/personadisplay.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.showMaximized();

    PersonaDisplay p;
    p.show();

    return QApplication::exec();
}
