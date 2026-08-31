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
    /**
     * @brief Builds the repository and seeds it with mock entries.
     * @param parent Parent object.
     */
    explicit EntryRepository(QObject *parent = nullptr);

    /**
     * @brief Gives read access to every entry currently held by the repository.
     * @return All stored entries.
     */
    const QVector<std::shared_ptr<Entry>> &entries() const;

    /**
     * @brief Looks up a single entry by its identifier.
     * @param id Identifier of the entry to find.
     * @return The matching entry, or nullptr if none was found.
     */
    std::shared_ptr<Entry> findById(const QString &id) const;

    /**
     * @brief Adds a new entry to the repository and notifies listeners.
     * @param entry Entry to add.
     */
    void addEntry(const std::shared_ptr<Entry> &entry);

    /**
     * @brief Removes the entry with the given identifier, if it exists, and notifies listeners.
     * @param id Identifier of the entry to remove.
     * @return True if an entry was found and removed, false otherwise.
     */
    bool removeEntry(const QString &id);

    /**
     * @brief Clears the given persona from every website entry currently linked to it, and notifies listeners if any entry changed.
     * @param personaId Identifier of the persona being unlinked.
     */
    void unlinkPersonaFromEntries(const QString &personaId);

signals:
    /**
     * @brief Emitted whenever an entry is added to or removed from the repository.
     */
    void entriesChanged();

private:
    QVector<std::shared_ptr<Entry>> m_entries;

    /**
     * @brief Fills the repository with placeholder entries until a real data source is wired in.
     */
    void seedMockData();
};

#endif // ENTRYREPOSITORY_H
