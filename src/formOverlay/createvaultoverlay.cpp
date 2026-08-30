#include "createvaultoverlay.h"
#include "ui_createvaultoverlay.h"

CreateVaultOverlay::CreateVaultOverlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateVaultOverlay)
{
    ui->setupUi(this);
    ui->vaultName->setLabelText("VAULT NAME");
    ui->masterPassword->setLabelText("MASTER PASSWORD");
    ui->confirmMasterPassword->setLabelText("CONFIRM MASTER PASSWORD");

    // By default, hide the overlay
    hide();

    // Buttons
    connect(ui->cancelCreation, &QPushButton::clicked, this, [this]() {
        clearForm();
        emit cancelled();
        hide();
    });

    connect(ui->createVaultButton, &QPushButton::clicked, this, [this]() {
        const QString name = ui->vaultName->text();
        const QString password = ui->masterPassword->text();
        const QString confirmPassword = ui->confirmMasterPassword->text();

        if (name.isEmpty() || password.isEmpty() || password != confirmPassword) {
            return;
        }

        emit vaultCreated(name, password);
        clearForm();
        hide();
    });
}

void CreateVaultOverlay::clearForm()
{
    ui->vaultName->setText("");
    ui->masterPassword->setText("");
    ui->confirmMasterPassword->setText("");
}

void CreateVaultOverlay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (parentWidget()) {
        setGeometry(parentWidget()->rect());
    }
}

CreateVaultOverlay::~CreateVaultOverlay()
{
    delete ui;
}
