#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QTransform>
#include <QPushButton>
#include <QResizeEvent>
#include <QAction>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>

// App config / vault bridge
#include "appconfig.h"
#include "vaultbridge.h"

// Sidebar imports
#include "sideBar/vaultselection.h"
#include "sideBar/categoriesselection.h"

// FormOverlay imports
#include "formOverlay/createvaultoverlay.h"
#include "formOverlay/newentryoverlay.h"
#include "formOverlay/createcategoryoverlay.h"
#include "formOverlay/editpersonaoverlay.h"

// Component imports
#include "component/notificationtooltip.h"

// Settings imports
#include "settings/settingswindow.h"

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
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    const QString vaultStoragePath = AppConfig::load().vaultStoragePath;
    m_vaultBridge = std::make_unique<VaultBridge>(vaultStoragePath);

    auto *settingsMenu = menuBar()->addMenu("Settings");
    auto *settingsAction = settingsMenu->addAction("Open Settings...");
    settingsAction->setMenuRole(QAction::NoRole);
    settingsAction->setShortcut(QKeySequence::Preferences);

    connect(settingsAction, &QAction::triggered, this, [this]() {
        SettingsWindow settingsWindow(this);
        settingsWindow.exec();
    });

    auto vaultSelection = new VaultSelection(this);
    ui->sideBar->addWidget(vaultSelection);
    ui->sideBar->setCurrentWidget(vaultSelection);

    const QStringList existingVaultFiles =
        QDir(vaultStoragePath).entryList(QStringList() << "*.kvdb", QDir::Files, QDir::Name);
    for (const QString &vaultFile : existingVaultFiles) {
        vaultSelection->addVault(QFileInfo(vaultFile).completeBaseName());
    }

    m_createVaultOverlay = new CreateVaultOverlay(this);
    m_createVaultOverlay->setGeometry(this->rect());
    m_createVaultOverlay->hide();

    connect(m_createVaultOverlay, &CreateVaultOverlay::vaultCreated, this, [this, vaultSelection](const QString &name, const QString &password) {
        if (QFileInfo::exists(m_vaultBridge->vaultFilePath(name))) {
            NotificationTooltip::showErrorToast(this, "A vault with this name already exists.");
            return;
        }

        if (!m_vaultBridge->createVault(name, password)) {
            NotificationTooltip::showErrorToast(this, "Failed to create the vault.");
            return;
        }

        m_createVaultOverlay->hide();
        m_createVaultOverlay->clearForm();
        vaultSelection->addVault(name);
        NotificationTooltip::showSuccessToast(this, "Vault created successfully.");
        });

    auto categoriesSelection = new CategoriesSelection(this);
    ui->sideBar->addWidget(categoriesSelection);

    auto createCategoryOverlay = new CreateCategoryOverlay(this);
    createCategoryOverlay->setGeometry(this->rect());
    createCategoryOverlay->hide();

    connect(categoriesSelection, &CategoriesSelection::createCategoryRequested, this, [this, createCategoryOverlay]() {
        createCategoryOverlay->setGeometry(this->rect());
        createCategoryOverlay->raise();
        createCategoryOverlay->show();
        });

    connect(createCategoryOverlay, &CreateCategoryOverlay::categoryCreated, this, [this, categoriesSelection, createCategoryOverlay](const QString &name) {
        if (!m_vaultBridge->addCategory(name)) {
            NotificationTooltip::showErrorToast(this, "Failed to create the category.");
            return;
        }

        createCategoryOverlay->hide();
        createCategoryOverlay->clearForm();

        categoriesSelection->setCategories(m_vaultBridge->categories());

        NotificationTooltip::showSuccessToast(this, "Category created successfully.");
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

    connect(unlockVaultModal, &unlockvaultmodal::unlockAttempted, this, [this, unlockVaultModal](const QString &vaultName, const QString &password) {
        switch (m_vaultBridge->unlockVault(vaultName, password)) {
        case VaultBridge::UnlockResult::Success:
            unlockVaultModal->reportUnlockSuccess();
            break;
        case VaultBridge::UnlockResult::VaultFileNotFound:
            unlockVaultModal->reportUnlockError("Vault file not found.");
            break;
        case VaultBridge::UnlockResult::IncorrectPasswordOrCorrupted:
            unlockVaultModal->reportUnlockError("Incorrect master password.");
            break;
        }
        });

    auto viewEntries = new ViewEntries(this);
    ui->mainContent->addWidget(viewEntries);
    viewEntries->setStyleSheet("background-color: #111827;");
    viewEntries->setVaultBridge(m_vaultBridge.get());

    connect(unlockVaultModal, &unlockvaultmodal::vaultUnlocked, this, [this, viewEntries, categoriesSelection](const QString &name) {
        categoriesSelection->setVaultName(name);

        const auto categories = m_vaultBridge->categories();
        categoriesSelection->setCategories(categories);

        if (categories.isEmpty()) {
            viewEntries->clearCategory();
        }

        ui->sideBar->setCurrentWidget(categoriesSelection);
        ui->mainContent->setProperty("openEntries", true);
        ui->mainContent->setCurrentWidget(viewEntries);
        });

    connect(categoriesSelection, &CategoriesSelection::lockVaultRequested, this, [this, vaultSelection, noVaultSelected]() {
        if (!m_vaultBridge->lockVault()) {
            NotificationTooltip::showErrorToast(this, "Failed to lock the vault.");
            return;
        }

        ui->sideBar->setCurrentWidget(vaultSelection);
        ui->mainContent->setProperty("openEntries", false);
        ui->mainContent->setCurrentWidget(noVaultSelected);
        });

    auto newEntryOverlay = new NewEntryOverlay(this);
    newEntryOverlay->setGeometry(this->rect());
    newEntryOverlay->hide();

    connect(viewEntries, &ViewEntries::newEntry, this, [this, newEntryOverlay]() {
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

    connect(newEntryOverlay, &NewEntryOverlay::newWifiEntryRequested, this, [this, newEntryOverlay, wifiForm]() {
        newEntryOverlay->hide();
        ui->mainContent->setCurrentWidget(wifiForm);
        });

    connect(newEntryOverlay, &NewEntryOverlay::newCreditCardEntryRequested, this, [this, newEntryOverlay, creditCardForm]() {
        newEntryOverlay->hide();
        ui->mainContent->setCurrentWidget(creditCardForm);
        });

    connect(newEntryOverlay, &NewEntryOverlay::newWebsiteCredentialsEntryRequested, this, [this, newEntryOverlay, addWebsiteForm]() {
        newEntryOverlay->hide();
        ui->mainContent->setCurrentWidget(addWebsiteForm);
        });

    connect(addWebsiteForm, &AddWebsiteForm::cancelNewWebEntry, this, [this, viewEntries]() {
        ui->mainContent->setCurrentWidget(viewEntries);
        });

    connect(creditCardForm, &CreditCardForm::cancelRequested, this, [this, viewEntries]() {
        ui->mainContent->setCurrentWidget(viewEntries);
        });

    connect(wifiForm, &WifiForm::cancelRequested, this, [this, viewEntries]() {
        ui->mainContent->setCurrentWidget(viewEntries);
        });

    connect(addWebsiteForm, &AddWebsiteForm::createWebsiteEntry, this,
            [this, viewEntries, addWebsiteForm](const QString &title, const QString &username, const QString &password,
                                 const QString &url, const QString &description, const QString &notes) {
        if (!viewEntries->createWebsiteEntry(title, username, password, url, description, notes)) {
            NotificationTooltip::showErrorToast(this, "Failed to create the website entry.");
            return;
        }
        addWebsiteForm->clearForm();
        ui->mainContent->setCurrentWidget(viewEntries);
        NotificationTooltip::showSuccessToast(this, "Website entry created successfully.");
    });

    connect(creditCardForm, &CreditCardForm::createCreditCardEntry, this,
            [this, viewEntries, creditCardForm](const QString &cardHolderName, const QString &cardNumber,
                                 const QString &expiration, const QString &securityCode, const QString &notes) {
        if (!viewEntries->createCreditCardEntry(cardHolderName, cardNumber, expiration, securityCode, notes)) {
            NotificationTooltip::showErrorToast(this, "Failed to create the credit card entry.");
            return;
        }
        creditCardForm->clearForm();
        ui->mainContent->setCurrentWidget(viewEntries);
        NotificationTooltip::showSuccessToast(this, "Credit card entry created successfully.");
    });

    connect(wifiForm, &WifiForm::createNewWifiEntry, this,
            [this, viewEntries, wifiForm](const QString &networkName, const QString &password, const QString &notes) {
        if (!viewEntries->createWifiEntry(networkName, password, notes)) {
            NotificationTooltip::showErrorToast(this, "Failed to create the wifi entry.");
            return;
        }
        wifiForm->clearForm();
        ui->mainContent->setCurrentWidget(viewEntries);
        NotificationTooltip::showSuccessToast(this, "Wifi entry created successfully.");
    });

    auto passwordGeneratorOverlay = new PasswordGenerator(this);
    ui->mainContent->addWidget(passwordGeneratorOverlay);

    connect(addWebsiteForm, &AddWebsiteForm::generatePassword, this, [this, passwordGeneratorOverlay]() {
        ui->mainContent->setCurrentWidget(passwordGeneratorOverlay);
        });

    connect(passwordGeneratorOverlay, &PasswordGenerator::usePasswordEvent, this, [this, addWebsiteForm](const QString password) {
        addWebsiteForm->setPassword(password);
        ui->mainContent->setCurrentWidget(addWebsiteForm);
        });

    connect(passwordGeneratorOverlay, &PasswordGenerator::cancelRequested, this, [this, addWebsiteForm]() {
        ui->mainContent->setCurrentWidget(addWebsiteForm);
        });

    auto personaDisplay = new PersonaDisplay(this);
    ui->mainContent->addWidget(personaDisplay);
    personaDisplay->setVaultBridge(m_vaultBridge.get());

    connect(categoriesSelection, &CategoriesSelection::setPersonaFrame, this, [this, personaDisplay]() {
        personaDisplay->loadPersonas();
        ui->mainContent->setCurrentWidget(personaDisplay);
        });

    connect(categoriesSelection, &CategoriesSelection::categorySelected, this, [this, viewEntries](qint64 categoryId) {
        viewEntries->loadCategory(categoryId);
        ui->mainContent->setCurrentWidget(viewEntries);
        });

    connect(categoriesSelection, &CategoriesSelection::categoryReselected, this, [this, viewEntries]() {
        viewEntries->clearSelection();
        ui->mainContent->setCurrentWidget(viewEntries);
        });

    auto personaForm = new NewPersonaForm(this);
    ui->mainContent->addWidget(personaForm);

    connect(personaDisplay, &PersonaDisplay::createPersona, this, [this, personaForm]() {
        ui->mainContent->setCurrentWidget(personaForm);
        });

    connect(personaForm, &NewPersonaForm::cancelSignal, this, [this, personaDisplay]() {
        ui->mainContent->setCurrentWidget(personaDisplay);
        });

    connect(personaForm, &NewPersonaForm::usePersonaSignal, this,
            [this, personaDisplay](const QString &firstName, const QString &lastName, const QDate &dateOfBirth,
                                    const QString &address, const QString &phone) {
        if (!personaDisplay->addPersona(firstName, lastName, dateOfBirth, address, phone)) {
            NotificationTooltip::showErrorToast(this, "Failed to create the persona.");
            return;
        }
        ui->mainContent->setCurrentWidget(personaDisplay);
        NotificationTooltip::showSuccessToast(this, "Persona created successfully.");
        });

    auto editPersonaOverlay = new EditPersonaOverlay(this);
    editPersonaOverlay->setGeometry(this->rect());
    editPersonaOverlay->hide();

    connect(personaDisplay, &PersonaDisplay::modifyPersonaRequested, this, [this, editPersonaOverlay](const VaultBridge::PersonaSummary &persona) {
        editPersonaOverlay->setPersona(persona);
        editPersonaOverlay->setGeometry(this->rect());
        editPersonaOverlay->raise();
        editPersonaOverlay->show();
        });

    connect(editPersonaOverlay, &EditPersonaOverlay::personaModified, this,
            [personaDisplay](qint64 id, const QString &firstName, const QString &lastName,
                              const QDate &dateOfBirth, const QString &address, const QString &phone) {
        if (!personaDisplay->updatePersona(id, firstName, lastName, dateOfBirth, address, phone)) {
            NotificationTooltip::showErrorToast(personaDisplay, "Failed to update the persona.");
            return;
        }
        NotificationTooltip::showSuccessToast(personaDisplay, "Persona updated successfully.");
    });

    // Resolves a strange timing issue bug with the Stacked Widget component.
    connect(ui->mainContent, &QStackedWidget::currentChanged, this, [this](int) {
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

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (m_createVaultOverlay) {
        m_createVaultOverlay->setGeometry(this->rect());
    }
}

void MainWindow::changeEvent(QEvent *event) {
    QMainWindow::changeEvent(event);

    if (event->type() == QEvent::ActivationChange && isActiveWindow()) {
        if (auto *current = ui->mainContent->currentWidget()) {
            current->update();
        }
    }
}


MainWindow::~MainWindow() {
    delete ui;
}
