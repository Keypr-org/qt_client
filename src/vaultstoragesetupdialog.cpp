#include "vaultstoragesetupdialog.h"
#include "appconfig.h"

#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

VaultStorageSetupDialog::VaultStorageSetupDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Choose Vault Storage Location");
    setModal(true);
    setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);

    auto *title = new QLabel("Where should Keypr store your vaults?", this);
    title->setStyleSheet("font-weight: 600; font-size: 14px;");
    title->setWordWrap(true);

    auto *description = new QLabel(
        "This is required before you can use Keypr. Choose a folder on this "
        "computer where your vault files will be kept. You can change this "
        "later in Settings.",
        this);
    description->setWordWrap(true);

    m_pathLabel = new QLabel("No folder selected.", this);
    m_pathLabel->setWordWrap(true);
    m_pathLabel->setStyleSheet("color: palette(mid);");

    m_browseButton = new QPushButton("Choose Folder...", this);
    connect(m_browseButton, &QPushButton::clicked, this, &VaultStorageSetupDialog::browseForDirectory);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: #ef4444;");
    m_errorLabel->setWordWrap(true);
    m_errorLabel->hide();

    m_confirmButton = new QPushButton("Confirm", this);
    m_confirmButton->setEnabled(false);
    m_confirmButton->setDefault(true);
    connect(m_confirmButton, &QPushButton::clicked, this, &VaultStorageSetupDialog::confirm);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addSpacing(8);
    layout->addWidget(m_pathLabel);
    layout->addWidget(m_browseButton);
    layout->addWidget(m_errorLabel);
    layout->addSpacing(8);
    layout->addWidget(m_confirmButton);

    setMinimumWidth(420);
}

void VaultStorageSetupDialog::browseForDirectory() {
    const QString dir = QFileDialog::getExistingDirectory(
        this, "Choose Vault Storage Folder", QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty()) {
        return;
    }

    m_selectedPath = dir;
    m_pathLabel->setText(dir);
    m_pathLabel->setStyleSheet("");
    m_errorLabel->hide();
    m_confirmButton->setEnabled(true);
}

void VaultStorageSetupDialog::confirm() {
    if (m_selectedPath.isEmpty() || !QFileInfo(m_selectedPath).isDir()) {
        m_errorLabel->setText("Please choose a valid folder.");
        m_errorLabel->show();
        m_confirmButton->setEnabled(false);
        return;
    }

    AppConfig config = AppConfig::load();
    config.vaultStoragePath = m_selectedPath;
    if (!config.save()) {
        m_errorLabel->setText("Failed to save configuration. Please try again.");
        m_errorLabel->show();
        return;
    }

    QDialog::accept();
}

void VaultStorageSetupDialog::closeEvent(QCloseEvent *event) {
    event->ignore();
}

void VaultStorageSetupDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        event->ignore();
        return;
    }
    QDialog::keyPressEvent(event);
}

void VaultStorageSetupDialog::reject() {
    // Intentionally does nothing: the user cannot dismiss this dialog without
    // choosing a valid vault storage folder first.
}
