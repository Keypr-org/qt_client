#include "cardnumberutils.h"

const int SPACE_FORMAT  = 6;
const int GROUP_FORMAT  = 4;
const QChar MASKED_CHAR = QChar(0x2022);

namespace CardNumberUtils

{

QString format(const QString &cardNumber)
{
    QString result;

    const int firstGroupSize = cardNumber.length() % GROUP_FORMAT;

    for (int i = 0; i < cardNumber.length(); ++i)
    {
        if (i > 0 && (i - firstGroupSize) % GROUP_FORMAT == 0)
            result += QString(SPACE_FORMAT, ' ');

        result += cardNumber[i];
    }

    return result;
}

QString mask(const QString &cardNumber)
{

    if (cardNumber.length() <= GROUP_FORMAT)
        return cardNumber;

    QString masked(cardNumber.length() - GROUP_FORMAT, MASKED_CHAR);
    masked += cardNumber.last(GROUP_FORMAT);

    return format(masked);
}
}
