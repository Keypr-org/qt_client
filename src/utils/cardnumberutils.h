#ifndef CARDNUMBERUTILS_H
#define CARDNUMBERUTILS_H

#include <QString>

namespace CardNumberUtils
{
    /**
     * @brief Formats a raw card number into its human-readable, grouped representation.
     * @param cardNumber Raw card number digits.
     * @return Formatted card number.
     */
    QString format(const QString &cardNumber);

    /**
     * @brief Masks a card number, revealing only the digits meant to stay visible.
     * @param cardNumber Raw card number digits.
     * @return Masked card number.
     */
    QString mask(const QString &cardNumber);
}

#endif // CARDNUMBERUTILS_H
