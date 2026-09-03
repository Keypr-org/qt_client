#include "settingswindow.h"
#include "ui_settingswindow.h"

#include "component/notificationtooltip.h"
#include "mailaliascontroller.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
    , m_mailAliasController(std::make_unique<MailAliasController>())
    , m_appConfig(AppConfig::load()) {
    ui->setupUi(this);
    setWindowTitle("Settings");

    ui->sourceEmailInput->setLabelText("SOURCE EMAIL");
    ui->sourceEmailInput->setInputPlaceholder("you@example.com");

    ui->apiKeyInput->setLabelText("POSTSCALE API KEY");
    ui->apiKeyInput->setEyeIconEnabled(true);

    loadCurrentSettings();

    connect(ui->changeVaultPathButton, &QPushButton::clicked, this, &SettingsWindow::changeVaultPath);

    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->saveButton, &QPushButton::clicked, this, [this]() {
        const QString sourceEmail = ui->sourceEmailInput->text().trimmed();
        const QString apiKey = ui->apiKeyInput->text().trimmed();

        if (sourceEmail.isEmpty() != apiKey.isEmpty()) {
            ui->notificationTooltip->showError("Please fill in both the source email and API key, or leave both empty.");
            return;
        }

        if (!QFileInfo(m_pendingVaultPath).isDir()) {
            ui->notificationTooltip->showError("Please choose a valid vault storage folder.");
            return;
        }

        if (!m_mailAliasController->setCredentials(apiKey.toStdString(), sourceEmail.toStdString())) {
            ui->notificationTooltip->showError("Failed to save settings.");
            return;
        }

        // Reload after setCredentials() so we build on the copy it just persisted,
        // instead of the (now stale) one loaded when this dialog was opened.
        m_appConfig = AppConfig::load();
        if (m_pendingVaultPath != m_appConfig.vaultStoragePath) {
            m_appConfig.vaultStoragePath = m_pendingVaultPath;
            if (!m_appConfig.save()) {
                ui->notificationTooltip->showError("Failed to save the vault storage location.");
                return;
            }
        }

        NotificationTooltip::showSuccessToast(parentWidget() ? parentWidget() : this, "Settings saved successfully.");
        accept();
    });
}

void SettingsWindow::loadCurrentSettings() {
    ui->sourceEmailInput->setText(QString::fromStdString(m_mailAliasController->sourceEmail()));
    ui->apiKeyInput->setText(QString::fromStdString(m_mailAliasController->apiKey()));

    m_pendingVaultPath = m_appConfig.vaultStoragePath;
    ui->vaultPathValueLabel->setText(m_pendingVaultPath.isEmpty() ? "No folder selected." : m_pendingVaultPath);
}

void SettingsWindow::changeVaultPath() {
    const QString startDir = QFileInfo(m_pendingVaultPath).isDir() ? m_pendingVaultPath : QDir::homePath();
    const QString dir = QFileDialog::getExistingDirectory(
        this, "Choose Vault Storage Folder", startDir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty()) {
        return;
    }

    m_pendingVaultPath = dir;
    ui->vaultPathValueLabel->setText(dir);
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}
