#include "viewentries.h"
#include "component/entrieitem.h"
#include "ui_viewentries.h"


ViewEntries::ViewEntries(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewEntries)
{
    ui->setupUi(this);

    auto item1 = new EntrieItem(this);
    item1->setIcon(EntrieItem::IconType::WebSite);
    item1->setPrimaryInfo("GitHub");
    item1->setSecondaryInfo("KeypingSafe");

    auto item2 = new EntrieItem(this);
    item2->setIcon(EntrieItem::IconType::Wifi);
    item2->setPrimaryInfo("Sunrise");
    item2->setSecondaryInfo("");

    auto item3 = new EntrieItem(this);
    item3->setIcon(EntrieItem::IconType::CreditCard);
    item3->setPrimaryInfo("VISA");
    item3->setSecondaryInfo("Alex Morgan");

    QListWidgetItem *item11 = new QListWidgetItem(ui->entriesList);
    QListWidgetItem *item22 = new QListWidgetItem(ui->entriesList);
    QListWidgetItem *item33 = new QListWidgetItem(ui->entriesList);

    item11->setSizeHint(item1->sizeHint());
    item22->setSizeHint(item2->sizeHint());
    item33->setSizeHint(item3->sizeHint());

    ui->entriesList->setItemWidget(item11, item1);
    ui->entriesList->setItemWidget(item22, item2);
    ui->entriesList->setItemWidget(item33, item3);

}

ViewEntries::~ViewEntries()
{
    delete ui;
}

void ViewEntries::on_entriesList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous) {
        EntrieItem *previousItem =
            qobject_cast<EntrieItem *>(
                ui->entriesList->itemWidget(previous)
                );

        if (previousItem) {
            previousItem->setSelected(false);
        }
    }

    if (current) {
        EntrieItem *newItem =
            qobject_cast<EntrieItem *>(
                ui->entriesList->itemWidget(current)
                );

        if (newItem) {
            newItem->setSelected(true);
        }
    }
}

