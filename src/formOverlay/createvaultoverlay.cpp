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

    //setAttribute(Qt::WA_StyledBackground, true);

    // By default, hide the overlay
    hide();

    // Buttons
    connect(ui->cancelCreation, &QPushButton::clicked, this, [this]() {
        emit cancelled();
        hide();
    });

    connect(ui->createVaultButton, &QPushButton::clicked, this, [this]() {
        // TODO emit vaultCreated(...);
        hide();
    });
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
