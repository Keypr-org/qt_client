#include "settingswindow.h"
#include "ui_settingswindow.h"

#include "component/notificationtooltip.h"
#include "mailaliascontroller.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
    , m_mailAliasController(std::make_unique<MailAliasController>()) {
    ui->setupUi(this);
    setWindowTitle("Settings");

    ui->sourceEmailInput->setLabelText("SOURCE EMAIL");
    ui->sourceEmailInput->setInputPlaceholder("you@example.com");

    ui->apiKeyInput->setLabelText("POSTSCALE API KEY");
    ui->apiKeyInput->setEyeIconEnabled(true);

    loadCurrentSettings();

    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->saveButton, &QPushButton::clicked, this, [this]() {
        const QString sourceEmail = ui->sourceEmailInput->text().trimmed();
        const QString apiKey = ui->apiKeyInput->text().trimmed();

        if (sourceEmail.isEmpty() || apiKey.isEmpty()) {
            ui->notificationTooltip->showError("Please fill in both the source email and API key.");
            return;
        }

        if (!m_mailAliasController->setCredentials(apiKey.toStdString(), sourceEmail.toStdString())) {
            ui->notificationTooltip->showError("Failed to save settings.");
            return;
        }

        NotificationTooltip::showSuccessToast(parentWidget() ? parentWidget() : this, "Settings saved successfully.");
        accept();
    });
}

void SettingsWindow::loadCurrentSettings() {
    ui->sourceEmailInput->setText(QString::fromStdString(m_mailAliasController->sourceEmail()));
    ui->apiKeyInput->setText(QString::fromStdString(m_mailAliasController->apiKey()));
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}
