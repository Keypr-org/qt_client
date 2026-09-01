#include "createvaultoverlay.h"
#include "ui_createvaultoverlay.h"

#include "component/notificationtooltip.h"
#include "component/passwordinput.h"
#include "utils/passwordstrengthutils.h"

CreateVaultOverlay::CreateVaultOverlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateVaultOverlay)
{
    ui->setupUi(this);
    ui->vaultName->setLabelText("VAULT NAME");
    ui->masterPassword->setLabelText("MASTER PASSWORD");
    ui->confirmMasterPassword->setLabelText("CONFIRM MASTER PASSWORD");

    connect(ui->masterPassword, &PasswordInput::textChanged, this, &CreateVaultOverlay::updateStrengthDisplay);
    updateStrengthDisplay("");

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

        if (name.isEmpty() || password.isEmpty()) {
            ui->notificationTooltip->showError("Please fill in all fields.");
            return;
        }

        if (password != confirmPassword) {
            ui->notificationTooltip->showError("Passwords do not match.");
            return;
        }

        emit vaultCreated(name, password);
        clearForm();
        hide();
        NotificationTooltip::showSuccessToast(parentWidget(), "Vault created successfully.");
    });
}

void CreateVaultOverlay::updateStrengthDisplay(const QString &password)
{
    const PasswordStrengthUtils::Strength strength = PasswordStrengthUtils::evaluate(password);
    const QString color = PasswordStrengthUtils::color(strength);
    const int filledLines = PasswordStrengthUtils::filledBarCount(strength);

    ui->passEval->setText(PasswordStrengthUtils::label(strength) + " Password");
    ui->passEval->setStyleSheet("color: " + color + ";");

    QFrame *lines[] = { ui->lineEval_1, ui->lineEval_2, ui->lineEval_3, ui->lineEval_4 };
    for (int i = 0; i < 4; ++i) {
        const QString lineColor = i < filledLines ? color : "#1F2937";
        lines[i]->setStyleSheet("color: " + lineColor + ";");
    }
}

void CreateVaultOverlay::clearForm()
{
    ui->vaultName->setText("");
    ui->masterPassword->setText("");
    ui->confirmMasterPassword->setText("");
    ui->notificationTooltip->hideMessage();
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
