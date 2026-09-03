#pragma once

#include "QEntry.h"

class QCreditCard : public QEntry
{
public:
    QCreditCard(Entry *creditCard);

    const QString getCardHolderName() const;
    void setCardHolderName(const QString &cardHolderName);

    const QString getCardNumber() const;
    void setCardNumber(const QString &cardNumber);

    const QString getExpiration() const;
    void setExpiration(const QString &expiration);

    const QString getSecurityCode() const;
    void setSecurityCode(const QString &securityCode);

    EntryKind getKind() const override;
};