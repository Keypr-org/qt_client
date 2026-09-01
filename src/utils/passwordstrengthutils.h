#ifndef PASSWORDSTRENGTHUTILS_H
#define PASSWORDSTRENGTHUTILS_H

#include <QString>

namespace PasswordStrengthUtils
{
    enum class Strength
    {
        Weak,
        Poor,
        Strong,
        VeryStrong
    };

    /**
     * @brief Evaluates the strength of a password based on its length and the variety
     * of character classes it uses (lowercase, uppercase, digits, special characters).
     * @param password Password to evaluate.
     * @return The estimated strength level.
     */
    Strength evaluate(const QString &password);

    /**
     * @brief Returns the human-readable label for a strength level.
     * @param strength Strength level.
     * @return Display label, e.g. "Weak", "Very Strong".
     */
    QString label(Strength strength);

    /**
     * @brief Returns the display color associated with a strength level.
     * @param strength Strength level.
     * @return Hex color string, e.g. "#EF4444".
     */
    QString color(Strength strength);

    /**
     * @brief Returns how many of the 4 rating bars should be filled for a strength level.
     * @param strength Strength level.
     * @return Number of bars to fill, from 1 (Weak) to 4 (VeryStrong).
     */
    int filledBarCount(Strength strength);
}

#endif // PASSWORDSTRENGTHUTILS_H
