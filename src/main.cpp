#include "mainwindow.h"
#include "mainContent/entries/websiteentry.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.show();

    WebsiteEntry z;
    z.show();

    return QApplication::exec();
}
