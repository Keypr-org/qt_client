#include "unlockvaultmodal.h"
#include "ui_unlockvaultmodal.h"

#include "component/notificationtooltip.h"

unlockvaultmodal::unlockvaultmodal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::unlockvaultmodal)
{
    ui->setupUi(this);
    ui->passwordInput->setLabelText("MASTER PASSWORD");

    connect(ui->unlockVaultButtonModal, &QPushButton::clicked,
            this, &unlockvaultmodal::onUnlockClicked);
}

void unlockvaultmodal::setVaultName(const QString &name)
{
    m_vaultName = name;
    ui->unlockVaultTitle->setText(QString("Unlock '%1' Vault").arg(name));
}

void unlockvaultmodal::onUnlockClicked()
{
    // TODO: replace with real password validation via vaultController
    NotificationTooltip::showSuccessToast(this, "Vault unlocked successfully.");
    emit vaultUnlocked(m_vaultName);
}

unlockvaultmodal::~unlockvaultmodal()
{
    delete ui;
}