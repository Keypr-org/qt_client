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
#include "formOverlay/createcategoryoverlay.h"
#include "formOverlay/editpersonaoverlay.h"

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

    auto vaultSelection = new VaultSelection(this);
    ui->sideBar->addWidget(vaultSelection);
    ui->sideBar->setCurrentWidget(vaultSelection);

    m_createVaultOverlay = new CreateVaultOverlay(this);
    m_createVaultOverlay->setGeometry(this->rect());
    m_createVaultOverlay->hide();

    connect(m_createVaultOverlay, &CreateVaultOverlay::vaultCreated, this, [vaultSelection](const QString &name, const QString &password) {
        // Calling keypr-core here
        vaultSelection->addVault(name);
    });

    auto categoriesSelection = new CategoriesSelection(this);
    ui->sideBar->addWidget(categoriesSelection);

    auto createCategoryOverlay = new CreateCategoryOverlay(this);
    createCategoryOverlay->setGeometry(this->rect());
    createCategoryOverlay->hide();

    connect(categoriesSelection, &CategoriesSelection::createCategoryRequested, this, [this, createCategoryOverlay](){
        createCategoryOverlay->setGeometry(this->rect());
        createCategoryOverlay->raise();
        createCategoryOverlay->show();
    });

    connect(createCategoryOverlay, &CreateCategoryOverlay::categoryCreated, this, [categoriesSelection](const QString &name){
        categoriesSelection->addCategory(name);
    });

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

    connect(addWebsiteForm, &AddWebsiteForm::createWebsiteEntry, this, [this, viewEntries](std::shared_ptr<WebsiteEntryData> entry){
        viewEntries->repository()->addEntry(entry);
        viewEntries->refresh();
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(creditCardForm, &CreditCardForm::createCreditCardEntry, this, [this, viewEntries](std::shared_ptr<CreditCardEntryData> entry){
        viewEntries->repository()->addEntry(entry);
        viewEntries->refresh();
        ui->mainContent->setCurrentWidget(viewEntries);
    });

    connect(wifiForm, &WifiForm::createNewWifiEntry, this, [this, viewEntries](std::shared_ptr<WifiEntryData> entry){
        viewEntries->repository()->addEntry(entry);
        viewEntries->refresh();
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

    connect(personaForm, &NewPersonaForm::usePersonaSignal, this, [this, personaDisplay](const PersonaData &persona){
        ui->mainContent->setCurrentWidget(personaDisplay);
        personaDisplay->addPersona(persona);
    });

    auto editPersonaOverlay = new EditPersonaOverlay(this);
    editPersonaOverlay->setGeometry(this->rect());
    editPersonaOverlay->hide();

    connect(personaDisplay, &PersonaDisplay::modifyPersonaRequested, this, [this, editPersonaOverlay](const PersonaData &persona){
        editPersonaOverlay->setPersona(persona);
        editPersonaOverlay->setGeometry(this->rect());
        editPersonaOverlay->raise();
        editPersonaOverlay->show();
    });

    connect(editPersonaOverlay, &EditPersonaOverlay::personaModified, this, [personaDisplay](const PersonaData &persona){
        personaDisplay->updatePersona(persona);
    });

    // Resolves a strange timing issue bug with the Stacked Widget component.
    connect(ui->mainContent, &QStackedWidget::currentChanged, this, [this](int){
        auto *current = ui->mainContent->currentWidget();
        for (int i = 0; i < ui->mainContent->count(); ++i) {
            QWidget *page = ui->mainContent->widget(i);
            if (page != current) {
                page->hide();
            }
        }
        if (current) {
            current->show();
            current->update();
        }
    });
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (m_createVaultOverlay) {
        m_createVaultOverlay->setGeometry(this->rect());
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);

    if (event->type() == QEvent::ActivationChange && isActiveWindow()) {
        if (auto *current = ui->mainContent->currentWidget()) {
            current->update();
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}