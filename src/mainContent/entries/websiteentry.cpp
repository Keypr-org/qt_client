#include "websiteentry.h"
#include "ui_websiteentry.h"

#include "component/notificationtooltip.h"
#include "mailaliascontroller.h"

#include <QAction>
#include <QClipboard>
#include <QDateTime>
#include <QGuiApplication>
#include <QMenu>
#include <QPoint>
#include <QToolButton>

WebsiteEntry::WebsiteEntry(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WebsiteEntry)
{
    ui->setupUi(this);

    ui->usernameInput->setLabelText("USERNAME / EMAIL");
    ui->passwordInput->setLabelText("PASSWORD");
    ui->urlInput->setLabelText("URL");
    ui->descriptionInput->setLabelText("DESCRIPTION (OPTIONNAL)");
    ui->notesInput->setLabelText("NOTES (OPTIONNAL)");

    ui->descriptionInput->setInputPlaceholder("Enter description here...");
    ui->notesInput->setInputPlaceholder("Enter notes here...");

    ui->selectedPersonaName->setText("No persona linked");

    connect(ui->changeLink, &QPushButton::clicked, this, [this](){
        openPersonaPicker();
    });

    connect(ui->unlinkLink, &QPushButton::clicked, this, [this](){
        unlinkPersona();
    });

    connect(ui->deleteButton, &QPushButton::clicked, this, [this](){
        if (!m_entry) {
            return;
        }
        emit deleteRequested(m_entry->id);
    });

    connect(ui->generateAliasButton, &QPushButton::clicked, this, [this](){
        generateAlias();
    });

    connect(ui->deleteAliasButton, &QPushButton::clicked, this, [this](){
        deleteAlias();
    });

    connect(ui->copyAliasButton, &QToolButton::clicked, this, [this](){
        if (!m_entry || m_entry->aliasEmail.isEmpty()) {
            return;
        }
        QGuiApplication::clipboard()->setText(m_entry->aliasEmail);
        NotificationTooltip::showSuccessToast(this, "Alias copied to clipboard.");
    });

    connect(ui->applyButton, &QPushButton::clicked, this, [this](){
        if (!m_entry) {
            return;
        }

        if (ui->passwordInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Password cannot be empty.");
            return;
        }

        m_entry->username = ui->usernameInput->text();
        m_entry->password = ui->passwordInput->text();
        m_entry->url = ui->urlInput->text();
        m_entry->description = ui->descriptionInput->text();
        m_entry->notes = ui->notesInput->text();
        m_entry->secondaryInfo = m_entry->username;
        m_entry->lastUpdated = QDateTime::currentDateTime();

        ui->subtitle->setText(m_entry->url);

        emit entryUpdated(m_entry->id);
        NotificationTooltip::showSuccessToast(this, "Website entry updated successfully.");
    });
}

WebsiteEntry::~WebsiteEntry()
{
    delete ui;
}

void WebsiteEntry::setEntry(const std::shared_ptr<WebsiteEntryData> &entry)
{
    if (!entry) {
        return;
    }

    m_entry = entry;

    ui->title->setText(entry->primaryInfo);
    ui->subtitle->setText(entry->url);

    ui->usernameInput->setText(entry->username);
    ui->passwordInput->setText(entry->password);
    ui->urlInput->setText(entry->url);
    ui->descriptionInput->setText(entry->description);
    ui->notesInput->setText(entry->notes);

    refreshPersonaDisplay();
    refreshAliasDisplay();
}

void WebsiteEntry::setPersonaRepository(PersonaRepository *repository)
{
    m_personaRepository = repository;
}

void WebsiteEntry::refreshPersonaDisplay()
{
    if (!m_entry) {
        return;
    }

    PersonaData persona;
    const bool linked = !m_entry->personaId.isEmpty()
        && m_personaRepository
        && m_personaRepository->findById(m_entry->personaId, persona);

    if (linked) {
        ui->selectedPersonaName->setText(persona.firstName + " " + persona.lastName);
    } else {
        m_entry->personaId.clear();
        ui->selectedPersonaName->setText("No persona linked");
    }
}

void WebsiteEntry::openPersonaPicker()
{
    if (!m_entry) {
        return;
    }

    if (!m_personaRepository || m_personaRepository->personas().isEmpty()) {
        NotificationTooltip::showErrorToast(this, "No persona available. Create one first.");
        return;
    }

    QMenu menu(this);
    for (const auto &persona : m_personaRepository->personas()) {
        QAction *action = menu.addAction(persona.firstName + " " + persona.lastName);
        connect(action, &QAction::triggered, this, [this, persona](){
            m_entry->personaId = persona.id;
            refreshPersonaDisplay();
            emit entryUpdated(m_entry->id);
            NotificationTooltip::showSuccessToast(this, "Persona linked successfully.");
        });
    }

    menu.exec(ui->changeLink->mapToGlobal(QPoint(0, ui->changeLink->height())));
}

void WebsiteEntry::unlinkPersona()
{
    if (!m_entry) {
        return;
    }

    if (m_entry->personaId.isEmpty()) {
        NotificationTooltip::showErrorToast(this, "No persona linked to unlink.");
        return;
    }

    m_entry->personaId.clear();
    refreshPersonaDisplay();
    emit entryUpdated(m_entry->id);
    NotificationTooltip::showSuccessToast(this, "Persona unlinked successfully.");
}

void WebsiteEntry::generateAlias()
{
    if (!m_entry) {
        return;
    }

    MailAliasController mailAliasController;

    if (!mailAliasController.hasCredentials()) {
        NotificationTooltip::showErrorToast(this, "Set your Postscale API key and source email in Settings before generating an alias.");
        return;
    }

    const auto alias = mailAliasController.createAlias(m_entry->primaryInfo.toStdString());

    if (!alias) {
        NotificationTooltip::showErrorToast(this, QString("Failed to create alias: %1").arg(QString::fromStdString(mailAliasController.lastError())));
        return;
    }

    m_entry->aliasEmail = QString::fromStdString(alias->fullAddress());
    refreshAliasDisplay();
    emit entryUpdated(m_entry->id);
    NotificationTooltip::showSuccessToast(this, "Email alias created successfully.");
}

void WebsiteEntry::deleteAlias()
{
    if (!m_entry || m_entry->aliasEmail.isEmpty()) {
        return;
    }

    m_entry->aliasEmail.clear();
    refreshAliasDisplay();
    emit entryUpdated(m_entry->id);
    NotificationTooltip::showSuccessToast(this, "Alias removed from this entry.");
}

void WebsiteEntry::refreshAliasDisplay()
{
    const bool hasAlias = m_entry && !m_entry->aliasEmail.isEmpty();

    ui->aliasFormWrapper->setVisible(!hasAlias);
    ui->widgetAliasEmail->setVisible(hasAlias);
    ui->deleteAliasButton->setVisible(hasAlias);

    if (hasAlias) {
        ui->aliasEmailLabel->setText(m_entry->aliasEmail);
    }
}
