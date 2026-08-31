#include "wifientry.h"
#include "ui_wifientry.h"

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
        emit deleteRequested(m_entryId);
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

    m_entryId = entry->id;

    ui->titleLabel->setText(entry->ssid);
    ui->nameInput->setText(entry->ssid);
    ui->passwordInput->setText(entry->password);
    ui->notesInput->setText(entry->notes);
}
