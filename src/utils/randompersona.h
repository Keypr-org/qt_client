#ifndef RANDOMPERSONA_H
#define RANDOMPERSONA_H

#include <QStringList>

#include "model/persona.h"

/**
 * @brief Generates synthetic persona data (name, gender, birthday, country, address)
 * for use as fake test/demo identities.
 */
class RandomPersona
{
public:
    /**
     * @brief Generates a persona with randomized name, gender, birthday and, if a
     * non-empty country list is given, a randomly picked country.
     * @param countries Candidate countries to pick from; skipped when empty.
     * @return Randomly generated persona data (id left empty).
     */
    static PersonaData generate(const QStringList &countries = {});

private:
    RandomPersona() = delete;

    /**
     * @brief Picks a random gender.
     * @return "Male" or "Female".
     */
    static QString randomGender();

    /**
     * @brief Picks a random first name matching the given gender.
     * @param gender "Male" or "Female", as returned by randomGender().
     * @return Randomly picked first name.
     */
    static QString randomFirstName(const QString &gender);

    /**
     * @brief Picks a random last name.
     * @return Randomly picked last name.
     */
    static QString randomLastName();

    /**
     * @brief Picks a random birthday for a plausible adult age.
     * @return Random date between MAX_AGE_YEARS and MIN_AGE_YEARS ago.
     */
    static QDate randomBirthday();

    /**
     * @brief Builds a random but credible-looking street address.
     * @return Formatted address string, e.g. "123 Maple Street, Springfield, 54321".
     */
    static QString randomAddress();
};

#endif // RANDOMPERSONA_H
