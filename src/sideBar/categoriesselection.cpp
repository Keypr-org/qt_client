#include "categoriesselection.h"
#include "ui_categoriesselection.h"

const int LIST_MIN_HEIGHT = 0;
const int LIST_MAX_HEIGHT = 300;

const QString FOLDER_ICON_WHITE = ":/icons/icons/icon-folder.png";
const QString FOLDER_ICON_COLOR = ":/icons/icons/icon-open-folder.png";

CategoriesSelection::CategoriesSelection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CategoriesSelection)
{
    ui->setupUi(this);

    connect(ui->headerVaultCategories, &ClickableWidget::clicked, this, [this]() {
        bool isVisible = ui->listCategories->isVisible();
        ui->listCategories->setVisible(!isVisible);
        updateArrowIcon(!isVisible);
    });

    connect(ui->lockVault, &QPushButton::clicked, this, [this](){
        emit lockVaultRequested();
    });

    connect(ui->listCategories->model(),
            &QAbstractItemModel::rowsInserted,
            this,
            &CategoriesSelection::adjustListHeight);

    connect(ui->listCategories->model(),
            &QAbstractItemModel::rowsRemoved,
            this,
            &CategoriesSelection::adjustListHeight);

    adjustListHeight();
    ui->listCategories->setVisible(true);
    updateArrowIcon(true);

    QIcon lockIcon(FOLDER_ICON_WHITE);

    for (int i = 1; i <= 10; ++i) {
        QListWidgetItem *item = new QListWidgetItem(lockIcon, QString("Categorie%1").arg(i));
        ui->listCategories->addItem(item);
    }

    ui->listCategories->setCurrentRow(0);
    ui->listCategories->item(0)->setSelected(true);
}

void CategoriesSelection::adjustListHeight()
{
    int height = 0;

    for (int i = 0; i < ui->listCategories->count(); ++i) {
        height += ui->listCategories->sizeHintForRow(i);
    }

    height = qBound(LIST_MIN_HEIGHT, height, LIST_MAX_HEIGHT);

    ui->listCategories->setFixedHeight(height);
}

void CategoriesSelection::updateArrowIcon(bool expanded)
{
    QPixmap pixmap(":/icons/icons/chevron-down.png");
    if (!expanded) {
        QTransform rotation;
        rotation.rotate(-90);
        pixmap = pixmap.transformed(rotation, Qt::SmoothTransformation);
    }
    ui->iconArrow->setPixmap(pixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void CategoriesSelection::on_listCategories_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous) {
        previous->setIcon(QIcon(FOLDER_ICON_WHITE));
    }

    if (current) {
        current->setIcon(QIcon(FOLDER_ICON_COLOR));
    }
}

CategoriesSelection::~CategoriesSelection()
{
    delete ui;
}
