#include "QPersona.h"
#include <QDateTime>

QPersona::QPersona(const std::unique_ptr<Persona> &persona) : persona(persona) {}

const qint64 QPersona::getId() const
{
    return persona->getId();
}

const QString QPersona::getFirstName() const
{
    return QString::fromStdString(persona->getFirstName());
}

void QPersona::setFirstName(const QString &firstName)
{
    persona->setFirstName(firstName.toStdString());
}

const QString QPersona::getLastName() const
{
    return QString::fromStdString(persona->getLastName());
}

void QPersona::setLastName(const QString &lastName)
{
    persona->setLastName(lastName.toStdString());
}

const QDate QPersona::getDateOfBirth() const
{
    const qint64 days = toUnixMilliseconds(persona->getDateOfBirth()) / 86400000LL;
    return QDate::fromJulianDay(2440588 + days);
}

void QPersona::setDateOfBirth(const QDate &dateOfBirth)
{
    const qint64 days = dateOfBirth.toJulianDay() - 2440588;
    persona->setDateOfBirth(fromUnixMilliseconds(days * 86400000LL));
}

const QString QPersona::getAddress() const
{
    return QString::fromStdString(persona->getAddress());
}

void QPersona::setAddress(const QString &address)
{
    persona->setAddress(address.toStdString());
}

const QString QPersona::getPhone() const
{
    return QString::fromStdString(persona->getPhone());
}

void QPersona::setPhone(const QString &phone)
{
    persona->setPhone(phone.toStdString());
}