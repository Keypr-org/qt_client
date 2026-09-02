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

    connect(ui->personasButton, &QPushButton::clicked, this, [this](){
        emit setPersonaFrame();
        deselectAllCategories();
        setPersonaSelected(true);
    });

    connect(ui->listCategories, &QListWidget::itemClicked, this, [this](QListWidgetItem *item){
        const bool isReselect = (item == m_lastClickedCategory);
        m_lastClickedCategory = item;

        if (isReselect) {
            emit categoryReselected();
        }
    });

    connect(ui->createCategory, &QPushButton::clicked, this, [this](){
        emit createCategoryRequested();
    });
}

void CategoriesSelection::addCategory(qint64 id, const QString &name)
{
    auto *item = new QListWidgetItem(QIcon(FOLDER_ICON_WHITE), name);
    item->setData(Qt::UserRole, id);
    ui->listCategories->addItem(item);
}

void CategoriesSelection::setCategories(const QList<VaultBridge::CategorySummary> &categories)
{
    ui->listCategories->clear();
    m_lastClickedCategory = nullptr;

    for (const VaultBridge::CategorySummary &category : categories) {
        addCategory(category.id, category.name);
    }

    if (ui->listCategories->count() > 0) {
        ui->listCategories->setCurrentRow(0);
        ui->listCategories->item(0)->setSelected(true);
        m_lastClickedCategory = ui->listCategories->item(0);
    }
}

void CategoriesSelection::setVaultName(const QString &name)
{
    ui->vaultName->setText(name);
}

void CategoriesSelection::deselectAllCategories()
{
    for (int i = 0; i < ui->listCategories->count(); ++i) {
        QListWidgetItem *item = ui->listCategories->item(i);
        item->setIcon(QIcon(FOLDER_ICON_WHITE));
        item->setSelected(false);
    }

    ui->listCategories->setCurrentItem(nullptr);
    m_lastClickedCategory = nullptr;

    // Force an immediate repaint instead of leaving it to the next coalesced paint event.
    ui->listCategories->viewport()->update();
}

void CategoriesSelection::setPersonaSelected(bool selected)
{
    if (selected) {
        ui->personasButton->setIcon(QIcon(":/icons/icons/icon-personas-purple.png"));
        ui->personasButton->setStyleSheet(
            "#personasButton {"
            "text-align: left;"
            "padding: 10px 12px;"
            "background: #23193C;"
            "border: none;"
            "border-radius: 6px;"
            "color: #A91EE4;"
            "}"
            );
    } else {
        ui->personasButton->setIcon(QIcon(":/icons/icons/icon-personas.png"));
        ui->personasButton->setStyleSheet(
            "#personasButton {"
            "text-align: left;"
            "padding: 10px 12px;"
            "background: transparent;"
            "border: none;"
            "color: #9CA3AF;"
            "}"
            );
    }
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
        setPersonaSelected(false);
        emit categorySelected(current->data(Qt::UserRole).toLongLong());
    }
}

CategoriesSelection::~CategoriesSelection()
{
    delete ui;
}
