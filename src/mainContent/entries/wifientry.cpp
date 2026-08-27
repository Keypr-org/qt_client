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
}

WifiEntry::~WifiEntry()
{
    delete ui;
}
