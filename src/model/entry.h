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

    /**
     * @brief Destroys the entry, allowing derived entry types to clean up polymorphically.
     */
    virtual ~Entry() = default;

protected:
    /**
     * @brief Initializes the fields common to every entry kind; only callable by derived entry types.
     * @param id Unique entry identifier.
     * @param type Concrete kind of the entry.
     * @param primaryInfo Main display label for the entry.
     * @param secondaryInfo Secondary display label for the entry.
     * @param lastUpdated Timestamp of the last modification.
     */
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
