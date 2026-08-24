#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CryptoService.h"
#include <QDebug>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->headerRecentVaults->installEventFilter(this);
    ui->headerRecentVaults->setCursor(Qt::PointingHandCursor);

    ui->listVaults->setVisible(true);
    updateArrowIcon(true);

    try {
        // Test d'intégration keypr-core / libsodium
        CryptoService crypto;
    } catch (const std::exception& e) {
        qWarning() << "Erreur lors de l'initialisation :" << e.what();
    }

    QListWidgetItem *item_1 = new QListWidgetItem(QIcon(":/icons/icons/icon-lock.png"), "Vault1");
    ui->listVaults->addItem(item_1);
    QListWidgetItem *item_2 = new QListWidgetItem(QIcon(":/icons/icons/icon-lock.png"), "Vault2");
    ui->listVaults->addItem(item_2);
    QListWidgetItem *item_3 = new QListWidgetItem(QIcon(":/icons/icons/icon-lock.png"), "Vault3");
    ui->listVaults->addItem(item_3);
    QListWidgetItem *item_4 = new QListWidgetItem(QIcon(":/icons/icons/icon-lock.png"), "Vault4");
    ui->listVaults->addItem(item_4);
    QListWidgetItem *item_5 = new QListWidgetItem(QIcon(":/icons/icons/icon-lock.png"), "Vault5");
    ui->listVaults->addItem(item_5);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->headerRecentVaults && event->type() == QEvent::MouseButtonPress) {
        bool isVisible = ui->listVaults->isVisible();
        ui->listVaults->setVisible(!isVisible);
        updateArrowIcon(!isVisible);
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::updateArrowIcon(bool expanded)
{
    QPixmap pixmap(":/icons/icons/chevron-down.png");
    if (!expanded) {
        QTransform rotation;
        rotation.rotate(-90);
        pixmap = pixmap.transformed(rotation, Qt::SmoothTransformation);
    }
    ui->iconArrow->setPixmap(pixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

MainWindow::~MainWindow()
{
    delete ui;
}