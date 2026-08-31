#include "wifientry.h"
#include "ui_wifientry.h"

#include "component/notificationtooltip.h"

#include <QDateTime>

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
        if (!m_entry) {
            return;
        }
        emit deleteRequested(m_entry->id);
    });

    connect(ui->applyButton, &QPushButton::clicked, this, [this](){
        if (!m_entry) {
            return;
        }

        if (ui->nameInput->text().isEmpty() || ui->passwordInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a network name and password.");
            return;
        }

        m_entry->ssid = ui->nameInput->text();
        m_entry->password = ui->passwordInput->text();
        m_entry->notes = ui->notesInput->text();
        m_entry->primaryInfo = m_entry->ssid;
        m_entry->lastUpdated = QDateTime::currentDateTime();

        ui->titleLabel->setText(m_entry->ssid);

        emit entryUpdated(m_entry->id);
        NotificationTooltip::showSuccessToast(this, "Wifi entry updated successfully.");
    });
}

WifiEntry::~WifiEntry()
{
    delete ui;
}

void WifiEntry::setEntry(const std::shared_ptr<WifiEntryData> &entry)
{
    if (!entry) {
        return;
    }

    m_entry = entry;

    ui->titleLabel->setText(entry->ssid);
    ui->nameInput->setText(entry->ssid);
    ui->passwordInput->setText(entry->password);
    ui->notesInput->setText(entry->notes);
}
