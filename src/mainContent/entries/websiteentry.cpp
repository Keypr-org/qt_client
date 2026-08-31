#include "websiteentry.h"
#include "ui_websiteentry.h"

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
        emit deleteRequested(m_entryId);
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

    m_entryId = entry->id;

    ui->title->setText(entry->primaryInfo);
    ui->subtitle->setText(entry->url);

    ui->usernameInput->setText(entry->username);
    ui->passwordInput->setText(entry->password);
    ui->urlInput->setText(entry->url);
    ui->descriptionInput->setText(entry->description);
    ui->notesInput->setText(entry->notes);
}
