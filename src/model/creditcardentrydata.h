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

    /**
     * @brief Constructs a fully-populated credit card entry.
     * @param id Unique entry identifier.
     * @param cardLabel Display name of the card (e.g. bank/network label).
     * @param ownerName Name of the cardholder.
     * @param cardNumber Card number.
     * @param expiration Card expiration date.
     * @param cvv Card verification value.
     * @param notes Optional free-form notes.
     * @param lastUpdated Timestamp of the last modification.
     */
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
