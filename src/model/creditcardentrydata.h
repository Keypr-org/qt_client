#ifndef CREDITCARDENTRYDATA_H
#define CREDITCARDENTRYDATA_H

#include "entry.h"

struct CreditCardEntryData : public Entry
{
    QString cardLabel;
    QString ownerName;
    QString cardNumber;
    QString expiration;
    QString cvv;
    QString notes;

    CreditCardEntryData(const QString &id, const QString &cardLabel,
                         const QString &ownerName, const QString &cardNumber,
                         const QString &expiration, const QString &cvv,
                         const QString &notes, const QDateTime &lastUpdated)
        : Entry(id, EntryType::CreditCard, cardLabel, ownerName, lastUpdated)
        , cardLabel(cardLabel)
        , ownerName(ownerName)
        , cardNumber(cardNumber)
        , expiration(expiration)
        , cvv(cvv)
        , notes(notes)
    {}
};

#endif // CREDITCARDENTRYDATA_H
