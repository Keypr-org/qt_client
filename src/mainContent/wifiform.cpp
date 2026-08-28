#include "wifiform.h"
#include "ui_wifiform.h"

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
        emit cancelRequested();
    });

    connect(ui->saveButton, &QPushButton::clicked, this, [this](){
        emit createNewWifiEntry();
    });
}

WifiForm::~WifiForm()
{
    delete ui;
}
