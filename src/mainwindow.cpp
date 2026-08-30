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
#include "mainContent/creditcardform.h"
#include "mainContent/wifiform.h"
#include "mainContent/passwordgenerator.h"
#include "mainContent/personadisplay.h"
#include "mainContent/newpersonaform.h"

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

    auto creditCardForm = new CreditCardForm(this);
    ui->mainContent->addWidget(creditCardForm);

    auto wifiForm = new WifiForm(this);
    ui->mainContent->addWidget(wifiForm);

    connect(newEntryOverlay, &NewEntryOverlay::newWifiEntryRequested, this, [this, newEntryOverlay, wifiForm](){
        newEntryOverlay->hide();
        ui->mainContent->setCurrentWidget(wifiForm);
    });

    connect(newEntryOverlay, &NewEntryOverlay::newCreditCardEntryRequested, this, [this, newEntryOverlay, creditCardForm](){
        newEntryOverlay->hide();
        ui->mainContent->setCurrentWidget(creditCardForm);
    });

    connect(newEntryOverlay, &NewEntryOverlay::newWebsiteCredentialsEntryRequested, this, [this, newEntryOverlay, addWebsiteForm](){
        newEntryOverlay->hide();
        ui->mainContent->setCurrentWidget(addWebsiteForm);
    });

    connect(addWebsiteForm, &AddWebsiteForm::cancelNewWebEntry, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(creditCardForm, &CreditCardForm::cancelRequested, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(wifiForm, &WifiForm::cancelRequested, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(addWebsiteForm, &AddWebsiteForm::createWebsiteEntry, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(creditCardForm, &CreditCardForm::createCreditCardEntry, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(wifiForm, &WifiForm::createNewWifiEntry, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    auto passwordGeneratorOverlay = new PasswordGenerator(this);
    ui->mainContent->addWidget(passwordGeneratorOverlay);

    connect(addWebsiteForm, &AddWebsiteForm::generatePassword, this, [this, passwordGeneratorOverlay](){
        ui->mainContent->setCurrentWidget(passwordGeneratorOverlay);
    });

    connect(passwordGeneratorOverlay, &PasswordGenerator::usePasswordEvent, this, [this, addWebsiteForm](const QString password){
        addWebsiteForm->setPassword(password);
        ui->mainContent->setCurrentWidget(addWebsiteForm);
    });

    connect(passwordGeneratorOverlay, &PasswordGenerator::cancelRequested, this, [this, addWebsiteForm](){
        ui->mainContent->setCurrentWidget(addWebsiteForm);
    });

    auto personaDisplay = new PersonaDisplay(this);
    ui->mainContent->addWidget(personaDisplay);

    connect(categoriesSelection, &CategoriesSelection::setPersonaFrame, this, [this, personaDisplay](){
        ui->mainContent->setCurrentWidget(personaDisplay);
    });

    connect(categoriesSelection, &CategoriesSelection::categorySelected, this, [this, viewEntries](){
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(categoriesSelection, &CategoriesSelection::categoryReselected, this, [this, viewEntries](){
        viewEntries->clearSelection();
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    auto personaForm = new NewPersonaForm(this);
    ui->mainContent->addWidget(personaForm);

    connect(personaDisplay, &PersonaDisplay::createPersona, this, [this, personaForm](){
        ui->mainContent->setCurrentWidget(personaForm);
    });

    connect(personaForm, &NewPersonaForm::cancelSignal, this, [this, personaDisplay](){
        ui->mainContent->setCurrentWidget(personaDisplay);
    });

    connect(personaForm, &NewPersonaForm::usePersonaSignal, this, [this, personaDisplay](){
        ui->mainContent->setCurrentWidget(personaDisplay);
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