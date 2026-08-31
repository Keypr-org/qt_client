#include "wifiform.h"
#include "ui_wifiform.h"

#include "component/notificationtooltip.h"

#include <QDateTime>
#include <QUuid>

WifiForm::WifiForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WifiForm)
{
    ui->setupUi(this);

    ui->nameInput->setLabelText("network name (ssid)");
    ui->passwordInput->setLabelText("password");
    ui->notesInput->setLabelText("notes (optional");
    ui->notesInput->setInputPlaceholder("Enter notes here...");

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        clearForm();
        emit cancelRequested();
    });

    connect(ui->saveButton, &QPushButton::clicked, this, [this](){
        if (ui->nameInput->text().isEmpty() || ui->passwordInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a network name and password.");
            return;
        }

        auto entry = std::make_shared<WifiEntryData>(
            QUuid::createUuid().toString(),
            ui->nameInput->text(),
            ui->passwordInput->text(),
            ui->notesInput->text(),
            QDateTime::currentDateTime());

        emit createNewWifiEntry(entry);
        clearForm();
    });
}

void WifiForm::clearForm()
{
    ui->nameInput->setText("");
    ui->passwordInput->setText("");
    ui->notesInput->setText("");
}

WifiForm::~WifiForm()
{
    delete ui;
}
