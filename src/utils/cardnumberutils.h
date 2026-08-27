#ifndef CARDNUMBERUTILS_H
#define CARDNUMBERUTILS_H

#include <QString>

namespace CardNumberUtils
{
    QString format(const QString &cardNumber);
    QString mask(const QString &cardNumber);
}

#endif // CARDNUMBERUTILS_H
