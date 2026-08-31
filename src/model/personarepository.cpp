#include "personarepository.h"

PersonaRepository::PersonaRepository(QObject *parent)
    : QObject(parent)
{
}

const QVector<PersonaData> &PersonaRepository::personas() const
{
    return m_personas;
}

bool PersonaRepository::findById(const QString &id, PersonaData &out) const
{
    for (const auto &persona : m_personas) {
        if (persona.id == id) {
            out = persona;
            return true;
        }
    }

    return false;
}

void PersonaRepository::addPersona(const PersonaData &persona)
{
    m_personas.append(persona);
}

void PersonaRepository::updatePersona(const PersonaData &persona)
{
    for (auto &existing : m_personas) {
        if (existing.id == persona.id) {
            existing = persona;
            return;
        }
    }
}

void PersonaRepository::removePersona(const QString &id)
{
    for (int i = 0; i < m_personas.size(); ++i) {
        if (m_personas.at(i).id == id) {
            m_personas.removeAt(i);
            return;
        }
    }
}
