#include "entryrepository.h"

#include "creditcardentrydata.h"
#include "websiteentrydata.h"
#include "wifientrydata.h"

EntryRepository::EntryRepository(QObject *parent)
    : QObject(parent)
{
    seedMockData();
}

const QVector<std::shared_ptr<Entry>> &EntryRepository::entries() const
{
    return m_entries;
}

std::shared_ptr<Entry> EntryRepository::findById(const QString &id) const
{
    for (const auto &entry : m_entries) {
        if (entry->id == id) {
            return entry;
        }
    }

    return nullptr;
}

void EntryRepository::addEntry(const std::shared_ptr<Entry> &entry)
{
    m_entries.append(entry);
    emit entriesChanged();
}

bool EntryRepository::removeEntry(const QString &id)
{
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries.at(i)->id == id) {
            m_entries.removeAt(i);
            emit entriesChanged();
            return true;
        }
    }

    return false;
}

void EntryRepository::seedMockData()
{
    m_entries.append(std::make_shared<WebsiteEntryData>(
        "website-1", "GitHub", "KeypingSafe", "hunter2",
        "https://github.com", "", "", QDateTime::currentDateTime()));

    m_entries.append(std::make_shared<WifiEntryData>(
        "wifi-1", "Sunrise", "sunrise-password", "",
        QDateTime::currentDateTime()));

    m_entries.append(std::make_shared<CreditCardEntryData>(
        "card-1", "VISA", "Alex Morgan", "4111111111111111",
        "12/27", "123", "", QDateTime::currentDateTime()));
}
