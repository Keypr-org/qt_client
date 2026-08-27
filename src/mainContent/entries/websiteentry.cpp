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
}

WebsiteEntry::~WebsiteEntry()
{
    delete ui;
}
