#ifndef ENTRY_H
#define ENTRY_H

#include <QDateTime>
#include <QString>

enum class EntryType
{
    Website,
    Wifi,
    CreditCard
};

// Common fields shared by every entry kind.
struct Entry
{
    QString id;
    EntryType type;
    QString primaryInfo;
    QString secondaryInfo;
    QDateTime lastUpdated;

    virtual ~Entry() = default;

protected:
    Entry(const QString &id, EntryType type,
          const QString &primaryInfo, const QString &secondaryInfo,
          const QDateTime &lastUpdated)
        : id(id)
        , type(type)
        , primaryInfo(primaryInfo)
        , secondaryInfo(secondaryInfo)
        , lastUpdated(lastUpdated)
    {}
};

#endif // ENTRY_H
