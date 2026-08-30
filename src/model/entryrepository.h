#ifndef ENTRYREPOSITORY_H
#define ENTRYREPOSITORY_H

#include <QObject>
#include <QVector>
#include <memory>

#include "entry.h"

// Once real data is available (persistence, keypr-core, ...), this is the class to
// back with an actual data source without touching the UI layer.
class EntryRepository : public QObject
{
    Q_OBJECT

public:
    explicit EntryRepository(QObject *parent = nullptr);

    const QVector<std::shared_ptr<Entry>> &entries() const;
    std::shared_ptr<Entry> findById(const QString &id) const;

    void addEntry(const std::shared_ptr<Entry> &entry);
    bool removeEntry(const QString &id);

signals:
    void entriesChanged();

private:
    QVector<std::shared_ptr<Entry>> m_entries;

    void seedMockData();
};

#endif // ENTRYREPOSITORY_H
