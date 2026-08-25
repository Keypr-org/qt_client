#include "vaultselection.h"
#include "src/sideBar/ui_vaultselection.h"
#include "utils/clickablewidget.h"

VaultSelection::VaultSelection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VaultSelection)
{
    ui->setupUi(this);

    connect(ui->headerRecentVaults, &ClickableWidget::clicked, this, [this]() {
        bool isVisible = ui->listVaults->isVisible();
        ui->listVaults->setVisible(!isVisible);
        updateArrowIcon(!isVisible);
    });

    connect(ui->createVault, &QPushButton::clicked, this, [this]() {
        emit createVaultRequested();
    });


    ui->listVaults->setVisible(true);
    updateArrowIcon(true);

    QIcon lockIcon(":/icons/icons/icon-lock.png");

    for (int i = 1; i <= 5; ++i) {
        QListWidgetItem *item = new QListWidgetItem(lockIcon, QString("Vault%1").arg(i));
        ui->listVaults->addItem(item);
    }

}

void VaultSelection::updateArrowIcon(bool expanded)
{
    QPixmap pixmap(":/icons/icons/chevron-down.png");
    if (!expanded) {
        QTransform rotation;
        rotation.rotate(-90);
        pixmap = pixmap.transformed(rotation, Qt::SmoothTransformation);
    }
    ui->iconArrow->setPixmap(pixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

VaultSelection::~VaultSelection()
{
    delete ui;
}
