#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CryptoService.h"
#include "mainContent/novaultselected.h"
#include "sideBar/vaultselection.h"
#include <QDebug>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try {
        // Test d'intégration keypr-core / libsodium
        CryptoService crypto;
    } catch (const std::exception& e) {
        qWarning() << "Erreur lors de l'initialisation :" << e.what();
    }

    auto vaultSelection = new VaultSelection(this);
    auto noVaultSelected = new NoVaultSelected(this);

    ui->sideBar->addWidget(vaultSelection);
    ui->sideBar->setCurrentWidget(vaultSelection);

    ui->mainContent->addWidget(noVaultSelected);
    ui->mainContent->setCurrentWidget(noVaultSelected);
}


MainWindow::~MainWindow()
{
    delete ui;
}