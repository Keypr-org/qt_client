#include "websiteentry.h"
#include "ui_websiteentry.h"

#include "component/notificationtooltip.h"
#include "mailaliascontroller.h"

#include <QAction>
#include <QClipboard>
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
        if (m_entryId.isEmpty()) {
            return;
        }
        emit deleteRequested(m_entryId);
    });

    connect(ui->generateAliasButton, &QPushButton::clicked, this, [this](){
        generateAlias();
    });

    connect(ui->deleteAliasButton, &QPushButton::clicked, this, [this](){
        deleteAlias();
    });

    connect(ui->copyAliasButton, &QToolButton::clicked, this, [this](){
        if (m_alias.isEmpty()) {
            return;
        }
        QGuiApplication::clipboard()->setText(m_alias);
        NotificationTooltip::showSuccessToast(this, "Alias copied to clipboard.");
    });

    connect(ui->applyButton, &QPushButton::clicked, this, [this](){
        if (m_entryId.isEmpty()) {
            return;
        }

        if (ui->passwordInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Password cannot be empty.");
            return;
        }

        emit entrySaveRequested(m_entryId, ui->usernameInput->text(), ui->passwordInput->text(),
                                 ui->urlInput->text(), ui->descriptionInput->text(), ui->notesInput->text());
    });
}

WebsiteEntry::~WebsiteEntry()
{
    delete ui;
}

void WebsiteEntry::setEntry(const VaultBridge::EntrySummary &entry)
{
    m_entryId = entry.id;
    m_personaId = entry.personaId;
    m_aliasId = entry.aliasId;
    m_alias = entry.alias;

    ui->title->setText(entry.title);
    ui->subtitle->setText(entry.url);

    ui->usernameInput->setText(entry.username);
    ui->passwordInput->setText(entry.password);
    ui->urlInput->setText(entry.url);
    ui->descriptionInput->setText(entry.comments);
    ui->notesInput->setText(entry.notes);

    refreshPersonaDisplay();
    refreshAliasDisplay();
}

void WebsiteEntry::setAvailablePersonas(const QList<VaultBridge::PersonaSummary> &personas)
{
    m_availablePersonas = personas;
    refreshPersonaDisplay();
}

void WebsiteEntry::refreshPersonaDisplay()
{
    if (m_personaId >= 0) {
        for (const auto &persona : m_availablePersonas) {
            if (persona.id == m_personaId) {
                ui->selectedPersonaName->setText(persona.firstName + " " + persona.lastName);
                return;
            }
        }
    }

    ui->selectedPersonaName->setText("No persona linked");
}

void WebsiteEntry::openPersonaPicker()
{
    if (m_entryId.isEmpty()) {
        return;
    }

    if (m_availablePersonas.isEmpty()) {
        NotificationTooltip::showErrorToast(this, "No persona available. Create one first.");
        return;
    }

    QMenu menu(this);
    for (const auto &persona : m_availablePersonas) {
        QAction *action = menu.addAction(persona.firstName + " " + persona.lastName);
        connect(action, &QAction::triggered, this, [this, persona](){
            emit personaLinkRequested(m_entryId, persona.id);
        });
    }

    menu.exec(ui->changeLink->mapToGlobal(QPoint(0, ui->changeLink->height())));
}

void WebsiteEntry::unlinkPersona()
{
    if (m_entryId.isEmpty()) {
        return;
    }

    if (m_personaId < 0) {
        NotificationTooltip::showErrorToast(this, "No persona linked to unlink.");
        return;
    }

    emit personaUnlinkRequested(m_entryId);
}

void WebsiteEntry::generateAlias()
{
    if (m_entryId.isEmpty()) {
        return;
    }

    MailAliasController mailAliasController;

    if (!mailAliasController.hasCredentials()) {
        NotificationTooltip::showErrorToast(this, "Set your Postscale API key and source email in Settings before generating an alias.");
        return;
    }

    const auto alias = mailAliasController.createAlias(ui->title->text().toStdString());

    if (!alias) {
        NotificationTooltip::showErrorToast(this, QString("Failed to create alias: %1").arg(QString::fromStdString(mailAliasController.lastError())));
        return;
    }

    emit aliasSetRequested(m_entryId, QString::fromStdString(alias->id), QString::fromStdString(alias->fullAddress()));
}

void WebsiteEntry::deleteAlias()
{
    if (m_entryId.isEmpty() || m_alias.isEmpty()) {
        return;
    }

    if (!m_aliasId.isEmpty()) {
        MailAliasController mailAliasController;

        if (!mailAliasController.hasCredentials()) {
            NotificationTooltip::showErrorToast(this, "Set your Postscale API key and source email in Settings before deleting an alias.");
            return;
        }

        if (!mailAliasController.deleteAlias(m_aliasId.toStdString())) {
            NotificationTooltip::showErrorToast(this, QString("Failed to delete alias: %1").arg(QString::fromStdString(mailAliasController.lastError())));
            return;
        }
    }

    emit aliasClearRequested(m_entryId);
}

void WebsiteEntry::refreshAliasDisplay()
{
    const bool hasAlias = !m_alias.isEmpty();

    ui->aliasFormWrapper->setVisible(!hasAlias);
    ui->widgetAliasEmail->setVisible(hasAlias);
    ui->deleteAliasButton->setVisible(hasAlias);

    if (hasAlias) {
        ui->aliasEmailLabel->setText(m_alias);
    }
}
