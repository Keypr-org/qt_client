#include "viewentries.h"
#include "component/entrieitem.h"
#include "mainContent/entries/websiteentry.h"
#include "mainContent/entries/wifientry.h"
#include "mainContent/entries/creditcardentry.h"
#include "model/websiteentrydata.h"
#include "model/wifientrydata.h"
#include "model/creditcardentrydata.h"
#include "ui_viewentries.h"

#include <QLabel>
#include <QVBoxLayout>

EntrieItem::IconType iconForType(EntryType type)
{
    switch (type) {
    case EntryType::Website:
        return EntrieItem::IconType::WebSite;
    case EntryType::Wifi:
        return EntrieItem::IconType::Wifi;
    case EntryType::CreditCard:
        return EntrieItem::IconType::CreditCard;
    }

    return EntrieItem::IconType::WebSite;
}

QString typeLabelForType(EntryType type)
{
    switch (type) {
    case EntryType::Website:
        return "Website";
    case EntryType::Wifi:
        return "Wifi";
    case EntryType::CreditCard:
        return "Card";
    }

    return QString();
}

ViewEntries::ViewEntries(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewEntries)
    , m_repository(new EntryRepository(this))
{
    ui->setupUi(this);

    m_emptyDetailPage = new QWidget(this);
    auto *emptyLayout = new QVBoxLayout(m_emptyDetailPage);
    auto *emptyLabel = new QLabel("Select an entry to view its details", m_emptyDetailPage);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setStyleSheet("color: #9CA3AF; font-size: 14px;");
    emptyLayout->addWidget(emptyLabel);

    m_websiteEntryView = new WebsiteEntry(this);
    m_wifiEntryView = new WifiEntry(this);
    m_creditCardEntryView = new CreditCardEntry(this);

    m_detailStack = new QStackedWidget(ui->oneEntryPage);
    m_detailStack->addWidget(m_emptyDetailPage);
    m_detailStack->addWidget(m_websiteEntryView);
    m_detailStack->addWidget(m_wifiEntryView);
    m_detailStack->addWidget(m_creditCardEntryView);
    m_detailStack->setCurrentWidget(m_emptyDetailPage);

    auto *detailLayout = new QVBoxLayout(ui->oneEntryPage);
    detailLayout->setContentsMargins(0, 0, 0, 0);
    detailLayout->addWidget(m_detailStack);

    populateList();

    connect(ui->newItemButton, &QPushButton::clicked, this, [this](){
        emit newEntry();
    });

    connect(ui->searchInput, &QLineEdit::textChanged, this, [this](const QString &text){
        m_searchFilter = text;
        populateList();
    });

    connect(ui->entriesList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item){
        const QString id = item->data(Qt::UserRole).toString();
        if (auto entry = m_repository->findById(id)) {
            showEntryDetails(entry);
        }
    });

    connect(m_websiteEntryView, &WebsiteEntry::deleteRequested, this, [this](const QString &id){
        handleDeleteRequested(id);
    });

    connect(m_wifiEntryView, &WifiEntry::deleteRequested, this, [this](const QString &id){
        handleDeleteRequested(id);
    });

    connect(m_creditCardEntryView, &CreditCardEntry::deleteRequested, this, [this](const QString &id){
        handleDeleteRequested(id);
    });

    connect(m_websiteEntryView, &WebsiteEntry::entryUpdated, this, [this](const QString &id){
        handleEntryUpdated(id);
    });

    connect(m_wifiEntryView, &WifiEntry::entryUpdated, this, [this](const QString &id){
        handleEntryUpdated(id);
    });

    connect(m_creditCardEntryView, &CreditCardEntry::entryUpdated, this, [this](const QString &id){
        handleEntryUpdated(id);
    });
}

ViewEntries::~ViewEntries()
{
    delete ui;
}

EntryRepository *ViewEntries::repository() const
{
    return m_repository;
}

void ViewEntries::refresh()
{
    populateList();
}

void ViewEntries::clearSelection()
{
    ui->entriesList->setCurrentItem(nullptr);
    m_detailStack->setCurrentWidget(m_emptyDetailPage);
}

void ViewEntries::populateList()
{
    while (ui->entriesList->count() > 0) {
        QListWidgetItem *item = ui->entriesList->item(0);
        delete ui->entriesList->itemWidget(item);
        delete ui->entriesList->takeItem(0);
    }

    for (const auto &entry : m_repository->entries()) {
        if (!matchesFilter(entry)) {
            continue;
        }

        auto *itemWidget = new EntrieItem(this);
        itemWidget->setIcon(iconForType(entry->type));
        itemWidget->setPrimaryInfo(entry->primaryInfo);
        itemWidget->setSecondaryInfo(entry->secondaryInfo);

        auto *listItem = new QListWidgetItem(ui->entriesList);
        listItem->setData(Qt::UserRole, entry->id);
        listItem->setSizeHint(itemWidget->sizeHint());

        ui->entriesList->setItemWidget(listItem, itemWidget);
    }
}

bool ViewEntries::matchesFilter(const std::shared_ptr<Entry> &entry) const
{
    if (m_searchFilter.isEmpty()) {
        return true;
    }

    if (entry->primaryInfo.contains(m_searchFilter, Qt::CaseInsensitive)) {
        return true;
    }

    if (entry->secondaryInfo.contains(m_searchFilter, Qt::CaseInsensitive)) {
        return true;
    }

    if (typeLabelForType(entry->type).contains(m_searchFilter, Qt::CaseInsensitive)) {
        return true;
    }

    return false;
}

void ViewEntries::showEntryDetails(const std::shared_ptr<Entry> &entry)
{
    switch (entry->type) {
    case EntryType::Website:
        m_websiteEntryView->setEntry(std::static_pointer_cast<WebsiteEntryData>(entry));
        m_detailStack->setCurrentWidget(m_websiteEntryView);
        break;

    case EntryType::Wifi:
        m_wifiEntryView->setEntry(std::static_pointer_cast<WifiEntryData>(entry));
        m_detailStack->setCurrentWidget(m_wifiEntryView);
        break;

    case EntryType::CreditCard:
        m_creditCardEntryView->setEntry(std::static_pointer_cast<CreditCardEntryData>(entry));
        m_detailStack->setCurrentWidget(m_creditCardEntryView);
        break;
    }
}

void ViewEntries::handleDeleteRequested(const QString &id)
{
    m_repository->removeEntry(id);
    refresh();
    clearSelection();
}

void ViewEntries::handleEntryUpdated(const QString &id)
{
    populateList();

    for (int i = 0; i < ui->entriesList->count(); ++i) {
        QListWidgetItem *item = ui->entriesList->item(i);
        if (item->data(Qt::UserRole).toString() == id) {
            ui->entriesList->setCurrentItem(item);
            break;
        }
    }
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
