#include "QCreditCard.h"
#include "entities/CreditCard.h"

QCreditCard::QCreditCard(Entry *creditCard) : QEntry(creditCard)
{
    if (dynamic_cast<CreditCard *>(creditCard) == nullptr)
    {
        throw std::invalid_argument("QCreditCard must be constructed with a CreditCard entry.");
    }
}

const QString QCreditCard::getCardHolderName() const
{
    return QString::fromStdString(dynamic_cast<CreditCard *>(entry)->getCardHolderName());
}

void QCreditCard::setCardHolderName(const QString &cardHolderName)
{
    dynamic_cast<CreditCard *>(entry)->setCardHolderName(cardHolderName.toStdString());
}

const QString QCreditCard::getCardNumber() const
{
    return QString::fromStdString(dynamic_cast<CreditCard *>(entry)->getCardNumber());
}

void QCreditCard::setCardNumber(const QString &cardNumber)
{
    dynamic_cast<CreditCard *>(entry)->setCardNumber(cardNumber.toStdString());
}

const QString QCreditCard::getExpiration() const
{
    return QString::fromStdString(dynamic_cast<CreditCard *>(entry)->getExpiration());
}

void QCreditCard::setExpiration(const QString &expiration)
{
    dynamic_cast<CreditCard *>(entry)->setExpiration(expiration.toStdString());
}

const QString QCreditCard::getSecurityCode() const
{
    return QString::fromStdString(dynamic_cast<CreditCard *>(entry)->getSecurityCode());
}

void QCreditCard::setSecurityCode(const QString &securityCode)
{
    dynamic_cast<CreditCard *>(entry)->setSecurityCode(securityCode.toStdString());
}

QEntry::EntryKind QCreditCard::getKind() const
{
    return EntryKind::CreditCard;
}