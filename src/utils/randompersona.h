#ifndef RANDOMPERSONA_H
#define RANDOMPERSONA_H

#include <QDate>
#include <QString>

/**
 * @brief Generates synthetic persona data (name, birthday, address, phone) for use as fake
 * test/demo identities. Fields mirror keypr-core's Persona entity: no gender or country, since
 * the real entity has neither.
 */
class RandomPersona
{
public:
    struct Generated {
        QString firstName;
        QString lastName;
        QDate birthday;
        QString address;
        QString phone;
    };

    /**
     * @brief Generates a persona with a randomized name, birthday, address and phone number.
     * @return Randomly generated persona data.
     */
    static Generated generate();

private:
    RandomPersona() = delete;

    /**
     * @brief Picks a random gender, used only to pick a matching first-name pool.
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

    /**
     * @brief Builds a random but credible-looking phone number.
     * @return Formatted phone number, e.g. "+1 555-123-4567".
     */
    static QString randomPhone();
};

#endif // RANDOMPERSONA_H
