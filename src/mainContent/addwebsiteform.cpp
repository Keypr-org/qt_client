#include "addwebsiteform.h"
#include "ui_addwebsiteform.h"

#include "component/notificationtooltip.h"

#include <QDateTime>
#include <QUuid>

AddWebsiteForm::AddWebsiteForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddWebsiteForm)
{
    ui->setupUi(this);

    ui->websiteNameInput->setLabelText("WEBSITE NAME");
    ui->usernameEmailInput->setLabelText("USERNAME/EMAIL");
    ui->passwordInput->setLabelText("PASSWORD");
    ui->urlInput->setLabelText("WEBSITE URL");
    ui->notesInput->setLabelText("NOTES (OPTIONAL)");
    ui->descriptionInput->setLabelText("DESCRIPTION (OPTIONAL)");

    ui->websiteNameInput->setInputPlaceholder("Keypr");
    ui->usernameEmailInput->setInputPlaceholder("john@doe.com");
    ui->urlInput->setInputPlaceholder("https://keypr-org.github.com/landing-page");
    ui->notesInput->setInputPlaceholder("Enter notes about security questions, alternative recovery codes, or metadata details here...");
    ui->descriptionInput->setInputPlaceholder("Enter a description for this entry...");

    connect(ui->generatePasswordButton, &QPushButton::clicked, this, [this](){
        emit generatePassword();
    });

    connect(ui->validateWebsiteEntryButton, &QPushButton::clicked, this, [this](){
        if (ui->websiteNameInput->text().isEmpty() || ui->passwordInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a website name and password.");
            return;
        }

        auto entry = std::make_shared<WebsiteEntryData>(
            QUuid::createUuid().toString(),
            ui->websiteNameInput->text(),
            ui->usernameEmailInput->text(),
            ui->passwordInput->text(),
            ui->urlInput->text(),
            ui->descriptionInput->text(),
            ui->notesInput->text(),
            QDateTime::currentDateTime());

        emit createWebsiteEntry(entry);
        clearForm();
    });

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        clearForm();
        emit cancelNewWebEntry();
    });
}

void AddWebsiteForm::setPassword(const QString &password)
{
    ui->passwordInput->setText(password);
}

void AddWebsiteForm::clearForm()
{
    ui->websiteNameInput->setText("");
    ui->usernameEmailInput->setText("");
    ui->passwordInput->setText("");
    ui->urlInput->setText("");
    ui->descriptionInput->setText("");
    ui->notesInput->setText("");
}

AddWebsiteForm::~AddWebsiteForm()
{
    delete ui;
}
