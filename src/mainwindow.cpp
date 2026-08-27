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
#include "formOverlay/newentryoverlay.h"

// Main Content imports
#include "mainContent/novaultselected.h"
#include "mainContent/viewentries.h"
#include "mainContent/unlockvaultmodal.h"
#include "mainContent/addwebsiteform.h"

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
    ui->sideBar->addWidget(vaultSelection);
    ui->sideBar->setCurrentWidget(vaultSelection);

    auto categoriesSelection = new CategoriesSelection(this);
    ui->sideBar->addWidget(categoriesSelection);

    connect(vaultSelection, &VaultSelection::createVaultRequested, this, [this]() {
        m_createVaultOverlay->setGeometry(this->rect());
        m_createVaultOverlay->raise();
        m_createVaultOverlay->show();
    });

    auto noVaultSelected = new NoVaultSelected(this);
    ui->mainContent->addWidget(noVaultSelected);
    ui->mainContent->setCurrentWidget(noVaultSelected);

    auto unlockVaultModal = new unlockvaultmodal(this);
    ui->mainContent->addWidget(unlockVaultModal);

    connect(vaultSelection, &VaultSelection::vaultSelected, this, [this, unlockVaultModal](const QString &vaultName) {
        unlockVaultModal->setVaultName(vaultName);
        ui->mainContent->setCurrentWidget(unlockVaultModal);
    });

    auto viewEntries = new ViewEntries(this);
    ui->mainContent->addWidget(viewEntries);
    viewEntries->setStyleSheet("background-color: #111827;");

    connect(unlockVaultModal, &unlockvaultmodal::vaultUnlocked, this, [this, viewEntries, categoriesSelection](){
        ui->sideBar->setCurrentWidget(categoriesSelection);
        ui->mainContent->setProperty("openEntries", true);
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(categoriesSelection, &CategoriesSelection::lockVaultRequested, this, [this, vaultSelection, noVaultSelected](){
        ui->sideBar->setCurrentWidget(vaultSelection);
        ui->mainContent->setProperty("openEntries", false);
        ui->mainContent->setCurrentWidget(noVaultSelected);
    });

    auto newEntryOverlay = new NewEntryOverlay(this);
    newEntryOverlay->setGeometry(this->rect());
    newEntryOverlay->hide();

    connect(viewEntries, &ViewEntries::newEntry, this, [this, newEntryOverlay](){
        newEntryOverlay->setGeometry(this->rect());
        newEntryOverlay->raise();
        newEntryOverlay->show();
    });

    auto addWebsiteForm = new AddWebsiteForm(this);
    ui->mainContent->addWidget(addWebsiteForm);

    connect(newEntryOverlay, &NewEntryOverlay::newWifiEntryRequested, this, [this, newEntryOverlay](){
        newEntryOverlay->hide();
    });

    connect(newEntryOverlay, &NewEntryOverlay::newCreditCardEntryRequested, this, [this, newEntryOverlay](){
        newEntryOverlay->hide();
    });

    connect(newEntryOverlay, &NewEntryOverlay::newWebsiteCredentialsEntryRequested, this, [this, newEntryOverlay, addWebsiteForm](){
        newEntryOverlay->hide();
        ui->mainContent->setCurrentWidget(addWebsiteForm);
    });

    connect(addWebsiteForm, &AddWebsiteForm::cancelNewWebEntry, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(addWebsiteForm, &AddWebsiteForm::createWebsiteEntry, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

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