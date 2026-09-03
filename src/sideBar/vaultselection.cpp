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

    connect(ui->listVaults, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        emit vaultSelected(item->text());
    });

    connect(ui->createVault, &QPushButton::clicked, this, [this]() {
        emit createVaultRequested();
    });

    ui->listVaults->setVisible(true);
    updateArrowIcon(true);
}

void VaultSelection::addVault(const QString &name)
{
    QIcon lockIcon(":/icons/icons/icon-lock.png");
    QListWidgetItem *item = new QListWidgetItem(lockIcon, name);
    ui->listVaults->addItem(item);
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