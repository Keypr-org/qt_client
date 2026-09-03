#pragma once

#include <entities/Entry.h>
#include <QString>
#include <QDateTime>

class QEntry
{

public:
    enum class EntryKind
    {
        Website,
        Wifi,
        CreditCard,
    };
    QEntry(Entry *entry);

    virtual ~QEntry() = default;

    const qint64 getId() const;

    const QString getNotes() const;
    void setNotes(const QString &notes);

    QDateTime getLastModifiedDate() const;

    virtual EntryKind getKind() const = 0;

protected:
    Entry *entry;
};