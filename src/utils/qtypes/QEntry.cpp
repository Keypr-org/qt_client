#include "QEntry.h"

QEntry::QEntry(Entry *entry) : entry(entry)
{
}

const qint64 QEntry::getId() const
{
    return entry->getId();
}

const QString QEntry::getNotes() const
{
    return QString::fromStdString(entry->getNotes());
}

void QEntry::setNotes(const QString &notes)
{
    entry->setNotes(notes.toStdString());
}

QDateTime QEntry::getLastModifiedDate() const
{
    return QDateTime::fromMSecsSinceEpoch(toUnixMilliseconds(entry->getLastModifiedDate()));
}