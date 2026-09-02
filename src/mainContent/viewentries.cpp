#include "viewentries.h"
#include "component/entrieitem.h"
#include "mainContent/entries/websiteentry.h"
#include "mainContent/entries/wifientry.h"
#include "mainContent/entries/creditcardentry.h"
#include "component/notificationtooltip.h"
#include "ui_viewentries.h"

#include <QLabel>
#include <QSet>
#include <QVBoxLayout>

#include <utility>

#include "vaultcontroller.h"

#include "../utils/qtypes/QWebsite.h"
#include "../utils/qtypes/QWifi.h"
#include "../utils/qtypes/QCreditCard.h"

namespace
{
    EntrieItem::IconType iconForKind(QEntry::EntryKind kind)
    {
        switch (kind)
        {
        case QEntry::EntryKind::Website:
            return EntrieItem::IconType::WebSite;
        case QEntry::EntryKind::Wifi:
            return EntrieItem::IconType::Wifi;
        case QEntry::EntryKind::CreditCard:
            return EntrieItem::IconType::CreditCard;
        }

        return EntrieItem::IconType::WebSite;
    }

    QString typeLabelForKind(QEntry::EntryKind kind)
    {
        switch (kind)
        {
        case QEntry::EntryKind::Website:
            return "Website";
        case QEntry::EntryKind::Wifi:
            return "Wifi";
        case QEntry::EntryKind::CreditCard:
            return "Card";
        }

        return QString();
    }

    QString maskedCardNumber(const QString &cardNumber)
    {
        return cardNumber.length() >= 4 ? "•••• " + cardNumber.right(4) : "•••• " + cardNumber;
    }

    QString relativeLastUpdateText(const QDateTime &lastUpdated)
    {
        const qint64 secondsAgo = qMax<qint64>(0, lastUpdated.secsTo(QDateTime::currentDateTime()));
        const qint64 hoursAgo = secondsAgo / 3600;

        if (hoursAgo < 24)
        {
            return QString("%1h").arg(hoursAgo);
        }

        return QString("%1d").arg(hoursAgo / 24);
    }

    QString primaryInfoFor(const std::unique_ptr<QEntry> &entry)
    {
        if (auto *websiteEntry = dynamic_cast<const QWebsite *>(entry.get()))
        {
            return websiteEntry->getTitle();
        }
        else if (auto *wifiEntry = dynamic_cast<const QWifi *>(entry.get()))
        {
            return wifiEntry->getNetworkName();
        }
        else if (auto *cardEntry = dynamic_cast<const QCreditCard *>(entry.get()))
        {
            return maskedCardNumber(cardEntry->getCardNumber());
        }

        return QString();
    }

    QString secondaryInfoFor(const std::unique_ptr<QEntry> &entry)
    {

        if (auto *websiteEntry = dynamic_cast<const QWebsite *>(entry.get()))
        {
            return websiteEntry->getUsername();
        }
        else if (auto *wifiEntry = dynamic_cast<const QWifi *>(entry.get()))
        {
            return QString();
        }
        else if (auto *cardEntry = dynamic_cast<const QCreditCard *>(entry.get()))
        {
            return cardEntry->getCardHolderName();
        }

        return QString();
    }
}

ViewEntries::ViewEntries(QWidget *parent)
    : QWidget(parent), ui(new Ui::ViewEntries)
{
    ui->setupUi(this);
    ui->newItemButton->setEnabled(false);

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

    connect(ui->newItemButton, &QPushButton::clicked, this, [this]()
            { emit newEntry(); });

    connect(ui->searchInput, &QLineEdit::textChanged, this, [this](const QString &text)
            {
        m_searchFilter = text;
        refreshEntries(); });

    connect(ui->entriesList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item)
            {
        const qint64 id = item->data(Qt::UserRole).toLongLong();
        if (const auto *entry = findEntry(id)) {
            showEntryDetails(*entry);
        } });

    connect(m_websiteEntryView, &WebsiteEntry::deleteRequested, this, [this](qint64 id)
            { handleDeleteRequested(id); });
    connect(m_wifiEntryView, &WifiEntry::deleteRequested, this, [this](qint64 id)
            { handleDeleteRequested(id); });
    connect(m_creditCardEntryView, &CreditCardEntry::deleteRequested, this, [this](qint64 id)
            { handleDeleteRequested(id); });

    connect(m_websiteEntryView, &WebsiteEntry::entrySaveRequested, this,
            [this](qint64 id, const QString &username, const QString &password,
                   const QString &url, const QString &description, const QString &notes)
            {
                auto &controller = VaultController::getInstance();
                if (!controller.updateWebsiteEntry(m_currentCategoryId, id, username, password, url, description, notes))
                {
                    NotificationTooltip::showErrorToast(this, "Failed to update the website entry.");
                    return;
                }
                reloadAndReselect(id);
                NotificationTooltip::showSuccessToast(this, "Website entry updated successfully.");
            });

    connect(m_wifiEntryView, &WifiEntry::entrySaveRequested, this,
            [this](qint64 id, const QString &networkName, const QString &password, const QString &notes)
            {
                auto &controller = VaultController::getInstance();
                if (!controller.updateWifiEntry(m_currentCategoryId, id, networkName, password, notes))
                {
                    NotificationTooltip::showErrorToast(this, "Failed to update the wifi entry.");
                    return;
                }
                reloadAndReselect(id);
                NotificationTooltip::showSuccessToast(this, "Wifi entry updated successfully.");
            });

    connect(m_creditCardEntryView, &CreditCardEntry::entrySaveRequested, this,
            [this](qint64 id, const QString &cardHolderName, const QString &cardNumber,
                   const QString &expiration, const QString &securityCode, const QString &notes)
            {
                auto &controller = VaultController::getInstance();
                if (!controller.updateCreditCardEntry(m_currentCategoryId, id, cardHolderName, cardNumber, expiration, securityCode, notes))
                {
                    NotificationTooltip::showErrorToast(this, "Failed to update the credit card entry.");
                    return;
                }
                reloadAndReselect(id);
                NotificationTooltip::showSuccessToast(this, "Credit card entry updated successfully.");
            });

    connect(m_websiteEntryView, &WebsiteEntry::personaLinkRequested, this, [this](qint64 id, qint64 personaId)
            {
        auto &controller = VaultController::getInstance();
        if (!controller.linkPersonaToEntry(personaId, m_currentCategoryId, id)) {
            NotificationTooltip::showErrorToast(this, "Failed to link the persona.");
            return;
        }
        reloadAndReselect(id);
        NotificationTooltip::showSuccessToast(this, "Persona linked successfully."); });

    connect(m_websiteEntryView, &WebsiteEntry::personaUnlinkRequested, this, [this](qint64 id)
            {
        auto &controller = VaultController::getInstance();
        if (!controller.linkPersonaToEntry(-1, m_currentCategoryId, id)) {
            NotificationTooltip::showErrorToast(this, "Failed to unlink the persona.");
            return;
        }
        reloadAndReselect(id);
        NotificationTooltip::showSuccessToast(this, "Persona unlinked successfully."); });

    connect(m_websiteEntryView, &WebsiteEntry::aliasSetRequested, this,
            [this](qint64 id, const QString &aliasId, const QString &alias)
            {
                auto &controller = VaultController::getInstance();
                if (!controller.setAliasForWebsite(m_currentCategoryId, id, aliasId, alias))
                {
                    NotificationTooltip::showErrorToast(this, "Failed to save the alias.");
                    return;
                }
                reloadAndReselect(id);
                NotificationTooltip::showSuccessToast(this, "Email alias created successfully.");
            });

    connect(m_websiteEntryView, &WebsiteEntry::aliasClearRequested, this, [this](qint64 id)
            {
        auto &controller = VaultController::getInstance();
        if (!controller.setAliasForWebsite(m_currentCategoryId, id, "", "")) {
            NotificationTooltip::showErrorToast(this, "Failed to remove the alias.");
            return;
        }
        reloadAndReselect(id);
        NotificationTooltip::showSuccessToast(this, "Alias removed from this entry."); });
}

ViewEntries::~ViewEntries()
{
    delete ui;
}

void ViewEntries::loadCategory(qint64 categoryId)
{
    m_currentCategoryId = categoryId;
    m_searchFilter.clear();
    ui->searchInput->setText("");
    ui->newItemButton->setEnabled(true);
    refreshEntries();
    clearSelection();
}

void ViewEntries::clearCategory()
{
    m_currentCategoryId = -1;
    m_searchFilter.clear();
    ui->searchInput->setText("");
    ui->newItemButton->setEnabled(false);
    refreshEntries();
    clearSelection();
}

bool ViewEntries::createWebsiteEntry(const QString &title, const QString &username, const QString &password,
                                     const QString &url, const QString &description, const QString &notes)
{
    auto &controller = VaultController::getInstance();

    if (!controller.addWebsiteEntry(m_currentCategoryId, title, username, password, url, description, notes))
    {
        return false;
    }

    loadCategory(m_currentCategoryId);
    // selectEntry(id);
    return true;
}

bool ViewEntries::createWifiEntry(const QString &networkName, const QString &password, const QString &notes)
{
    auto &controller = VaultController::getInstance();

    if (!controller.addWifiEntry(m_currentCategoryId, networkName, password, notes))
    {
        return false;
    }

    loadCategory(m_currentCategoryId);
    // selectEntry(id);
    return true;
}

bool ViewEntries::createCreditCardEntry(const QString &cardHolderName, const QString &cardNumber,
                                        const QString &expiration, const QString &securityCode,
                                        const QString &notes)
{
    auto &controller = VaultController::getInstance();

    if (!controller.addCreditCardEntry(m_currentCategoryId, cardHolderName, cardNumber, expiration, securityCode, notes))
    {
        return false;
    }

    loadCategory(m_currentCategoryId);
    // selectEntry(id);
    return true;
}

void ViewEntries::clearSelection()
{
    ui->entriesList->setCurrentItem(nullptr);
    m_detailStack->setCurrentWidget(m_emptyDetailPage);
}

void ViewEntries::populateList()
{
    while (ui->entriesList->count() > 0)
    {
        QListWidgetItem *item = ui->entriesList->item(0);
        delete ui->entriesList->itemWidget(item);
        delete ui->entriesList->takeItem(0);
    }

    for (qsizetype i = 0; i < m_entries.size(); ++i)
    {
        const auto &entry = m_entries.at(i);
        auto *itemWidget = new EntrieItem(this);
        itemWidget->setIcon(iconForKind(entry->getKind()));
        itemWidget->setPrimaryInfo(primaryInfoFor(entry));
        itemWidget->setSecondaryInfo(secondaryInfoFor(entry));
        itemWidget->setLastUpdate(relativeLastUpdateText(entry->getLastModifiedDate()));

        auto *listItem = new QListWidgetItem(ui->entriesList);
        listItem->setData(Qt::UserRole, entry->getId());
        listItem->setSizeHint(itemWidget->sizeHint());

        ui->entriesList->setItemWidget(listItem, itemWidget);
    }
}

void ViewEntries::refreshEntries()
{
    auto &controller = VaultController::getInstance();

    if (m_searchFilter.isEmpty())
    {
        m_entries = controller.getEntriesInCategory(m_currentCategoryId);
        populateList();
        return;
    }

    // VaultController::searchEntriesInCategory() matches entry content (notes, and per-kind
    // fields like title/username/url/comments/alias, network name, or cardholder name) but has
    // no concept of "kind" at all, so entries that only match by kind label (e.g. typing "wifi"
    // or "card") are merged in separately, client-side.
    m_entries = controller.searchEntriesInCategory(m_currentCategoryId, m_searchFilter);

    QSet<qint64> matchedIds;
    for (qsizetype i = 0; i < m_entries.size(); ++i)
    {
        const auto &entry = m_entries.at(i);
        matchedIds.insert(entry->getId());
    }

    for (auto &entry : controller.getEntriesInCategory(m_currentCategoryId))
    {
        if (matchedIds.contains(entry->getId()))
        {
            continue;
        }
        if (typeLabelForKind(entry->getKind()).contains(m_searchFilter, Qt::CaseInsensitive))
        {
            m_entries.push_back(std::move(entry));
        }
    }

    populateList();
}

const QEntry *ViewEntries::findEntry(qint64 id) const
{
    for (qsizetype i = 0; i < m_entries.size(); ++i)
    {
        const auto &entry = m_entries.at(i);
        if (entry->getId() == id)
        {
            return entry.get();
        }
    }
    return nullptr;
}

void ViewEntries::showEntryDetails(const QEntry &entry)
{
    auto &controller = VaultController::getInstance();
    switch (entry.getKind())
    {
    case QEntry::EntryKind::Website:
        m_websiteEntryView->setAvailablePersonas(controller.getPersonas());
        m_websiteEntryView->setEntry(entry);
        m_detailStack->setCurrentWidget(m_websiteEntryView);
        break;

    case QEntry::EntryKind::Wifi:
        m_wifiEntryView->setEntry(entry);
        m_detailStack->setCurrentWidget(m_wifiEntryView);
        break;

    case QEntry::EntryKind::CreditCard:
        m_creditCardEntryView->setEntry(entry);
        m_detailStack->setCurrentWidget(m_creditCardEntryView);
        break;
    }
}

void ViewEntries::selectEntry(qint64 id)
{
    for (int i = 0; i < ui->entriesList->count(); ++i)
    {
        QListWidgetItem *item = ui->entriesList->item(i);
        if (item->data(Qt::UserRole).toLongLong() == id)
        {
            ui->entriesList->setCurrentItem(item);
            // setCurrentItem() only updates the list row's visual selection; the detail pane
            // needs to be explicitly repopulated with the freshly-reloaded data (e.g. so a
            // just-created alias or a just-linked persona actually shows up after
            // reloadAndReselect(), instead of leaving the pane showing what it had before).
            if (const auto *entry = findEntry(id))
            {
                showEntryDetails(*entry);
            }
            return;
        }
    }
}

void ViewEntries::reloadAndReselect(qint64 id)
{
    refreshEntries();
    selectEntry(id);
}

void ViewEntries::handleDeleteRequested(qint64 id)
{
    auto &controller = VaultController::getInstance();
    if (!controller.removeEntryFromCategory(m_currentCategoryId, id))
    {
        NotificationTooltip::showErrorToast(this, "Failed to delete the entry.");
        return;
    }
    refreshEntries();
    clearSelection();
}

void ViewEntries::on_entriesList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous)
    {
        EntrieItem *previousItem =
            qobject_cast<EntrieItem *>(
                ui->entriesList->itemWidget(previous));

        if (previousItem)
        {
            previousItem->setSelected(false);
        }
    }

    if (current)
    {
        EntrieItem *newItem =
            qobject_cast<EntrieItem *>(
                ui->entriesList->itemWidget(current));

        if (newItem)
        {
            newItem->setSelected(true);
        }
    }
}
