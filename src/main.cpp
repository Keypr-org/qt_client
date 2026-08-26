#include "sideBar/categoriesselection.h"
#include "mainwindow.h"
#include "sideBar/vaultselection.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Keypr | Your local password manager");
    w.show();

    //CategoriesSelection u;
    //u.show();
    return QApplication::exec();
}
