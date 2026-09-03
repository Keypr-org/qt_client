#pragma once

#include "entities/Persona.h"
#include <QString>
#include <QDate>

class QPersona
{
public:
    QPersona(const std::unique_ptr<Persona> &persona);

    const qint64 getId() const;

    const QString getFirstName() const;
    void setFirstName(const QString &firstName);

    const QString getLastName() const;
    void setLastName(const QString &lastName);

    const QDate getDateOfBirth() const;
    void setDateOfBirth(const QDate &dateOfBirth);

    const QString getAddress() const;
    void setAddress(const QString &address);

    const QString getPhone() const;
    void setPhone(const QString &phone);

private:
    const std::unique_ptr<Persona> &persona;
};