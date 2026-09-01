#include "wifientry.h"
#include "ui_wifientry.h"

#include "component/notificationtooltip.h"

WifiEntry::WifiEntry(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WifiEntry)
{
    ui->setupUi(this);

    ui->nameInput->setLabelText("network name (ssid)");
    ui->passwordInput->setLabelText("password");
    ui->notesInput->setLabelText("notes (optional");
    ui->notesInput->setInputPlaceholder("Enter notes here...");

    connect(ui->deleteButton, &QPushButton::clicked, this, [this](){
        if (m_entryId.isEmpty()) {
            return;
        }
        emit deleteRequested(m_entryId);
    });

    connect(ui->applyButton, &QPushButton::clicked, this, [this](){
        if (m_entryId.isEmpty()) {
            return;
        }

        if (ui->nameInput->text().isEmpty() || ui->passwordInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a network name and password.");
            return;
        }

        emit entrySaveRequested(m_entryId, ui->nameInput->text(), ui->passwordInput->text(), ui->notesInput->text());
    });
}

WifiEntry::~WifiEntry()
{
    delete ui;
}

void WifiEntry::setEntry(const VaultBridge::EntrySummary &entry)
{
    m_entryId = entry.id;

    ui->titleLabel->setText(entry.networkName);
    ui->nameInput->setText(entry.networkName);
    ui->passwordInput->setText(entry.password);
    ui->notesInput->setText(entry.notes);
}
