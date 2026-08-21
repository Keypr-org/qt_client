#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CryptoService.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try {
        // Test d'intégration keypr-core / libsodium
        CryptoService crypto;
        qDebug() << "keypr-core chargé avec succès depuis qt_client !";
        qDebug() << "libsodium initialisée avec succès";
    } catch (const std::exception& e) {
        qWarning() << "Erreur lors de l'initialisation :" << e.what();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}