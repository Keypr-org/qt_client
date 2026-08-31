#ifndef PERSONAREPOSITORY_H
#define PERSONAREPOSITORY_H

#include <QObject>
#include <QVector>

#include "persona.h"

// Mirrors EntryRepository until a real data source is wired in
class PersonaRepository : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Builds an empty persona repository.
     * @param parent Parent object.
     */
    explicit PersonaRepository(QObject *parent = nullptr);

    /**
     * @brief Gives read access to every persona currently held by the repository.
     * @return All stored personas.
     */
    const QVector<PersonaData> &personas() const;

    /**
     * @brief Looks up a single persona by its identifier.
     * @param id Identifier of the persona to find.
     * @param out Filled with the matching persona's data if found.
     * @return True if a persona was found, false otherwise.
     */
    bool findById(const QString &id, PersonaData &out) const;

    /**
     * @brief Adds a new persona to the repository and notifies listeners.
     * @param persona Persona to add.
     */
    void addPersona(const PersonaData &persona);

    /**
     * @brief Replaces the stored data of an existing persona and notifies listeners.
     * @param persona Persona holding the id to match and the updated data.
     */
    void updatePersona(const PersonaData &persona);

    /**
     * @brief Removes the persona with the given identifier, if it exists, and notifies listeners.
     * @param id Identifier of the persona to remove.
     */
    void removePersona(const QString &id);

private:
    QVector<PersonaData> m_personas;
};

#endif // PERSONAREPOSITORY_H
