#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QTransform>
#include <QPushButton>
#include <QResizeEvent>

// Crypto
#include "CryptoService.h"

// Sidebar imports
#include "sideBar/vaultselection.h"
#include "sideBar/categoriesselection.h"

// FormOverlay imports
#include "formOverlay/createvaultoverlay.h"

// Main Content imports
#include "mainContent/novaultselected.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try {
        // Integration test keypr-core / libsodium
        CryptoService crypto;
    } catch (const std::exception& e) {
        qWarning() << "Erreur lors de l'initialisation :" << e.what();
    }

    m_createVaultOverlay = new CreateVaultOverlay(this);
    m_createVaultOverlay->setGeometry(this->rect());
    m_createVaultOverlay->hide();

    connect(m_createVaultOverlay, &CreateVaultOverlay::vaultCreated, this, [this](const QString &name, const QString &password) {
        qDebug() << "Vault to be created :" << name;
        // Calling keypr-core here
    });

    connect(m_createVaultOverlay, &CreateVaultOverlay::cancelled, this, [this]() {
        // rien de spécial, déjà caché dans l'overlay
    });

    auto vaultSelection = new VaultSelection(this);
    auto categoriesSelection = new CategoriesSelection(this);
    ui->sideBar->addWidget(vaultSelection);
    ui->sideBar->addWidget(categoriesSelection);
    ui->sideBar->setCurrentWidget(vaultSelection);

    connect(vaultSelection, &VaultSelection::createVaultRequested, this, [this]() {
        m_createVaultOverlay->setGeometry(this->rect());
        m_createVaultOverlay->raise();
        m_createVaultOverlay->show();
    });

    auto noVaultSelected = new NoVaultSelected(this);
    ui->mainContent->addWidget(noVaultSelected);
    ui->mainContent->setCurrentWidget(noVaultSelected);

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (m_createVaultOverlay) {
        m_createVaultOverlay->setGeometry(this->rect());
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}