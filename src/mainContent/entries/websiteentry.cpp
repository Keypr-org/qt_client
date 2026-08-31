#include "websiteentry.h"
#include "ui_websiteentry.h"

#include <QDateTime>

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

        m_entry->username = ui->usernameInput->text();
        m_entry->password = ui->passwordInput->text();
        m_entry->url = ui->urlInput->text();
        m_entry->description = ui->descriptionInput->text();
        m_entry->notes = ui->notesInput->text();
        m_entry->secondaryInfo = m_entry->username;
        m_entry->lastUpdated = QDateTime::currentDateTime();

        ui->subtitle->setText(m_entry->url);

        emit entryUpdated(m_entry->id);
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
}
