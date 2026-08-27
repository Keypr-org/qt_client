#include "addwebsiteform.h"
#include "ui_addwebsiteform.h"

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

    connect(ui->generatePasswordButton, &QPushButton::clicked, this, [this](){
        emit generatePassword();
    });

    connect(ui->validateWebsiteEntryButton, &QPushButton::clicked, this, [this](){
        emit createWebsiteEntry();
    });

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        emit cancelNewWebEntry();
    });
}

AddWebsiteForm::~AddWebsiteForm()
{
    delete ui;
}
