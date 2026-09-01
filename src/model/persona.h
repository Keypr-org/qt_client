#ifndef PERSONA_H
#define PERSONA_H

#include <QDate>
#include <QString>

struct PersonaData
{
    QString id;
    QString firstName;
    QString lastName;
    QDate birthday;
    QString gender;
    QString country;
    QString address;
};

#endif // PERSONA_H
