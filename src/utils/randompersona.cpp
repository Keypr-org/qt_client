#include "randompersona.h"

#include <QRandomGenerator>

namespace
{

/* AI generated palette of names and addresses */
const QStringList MALE_FIRST_NAMES = {
    "James", "John", "Robert", "Michael", "William", "David", "Richard", "Joseph",
    "Thomas", "Charles", "Daniel", "Matthew", "Anthony", "Mark", "Paul", "Steven",
    "Andrew", "Kevin", "Brian", "George", "Timothy", "Ronald", "Jason", "Edward",
    "Jeffrey", "Ryan", "Jacob", "Nicholas", "Alexander", "Benjamin"
};

const QStringList FEMALE_FIRST_NAMES = {
    "Mary", "Patricia", "Jennifer", "Linda", "Elizabeth", "Barbara", "Susan", "Jessica",
    "Sarah", "Karen", "Nancy", "Lisa", "Margaret", "Betty", "Sandra", "Ashley",
    "Emily", "Donna", "Michelle", "Amanda", "Melissa", "Deborah", "Stephanie", "Rebecca",
    "Sharon", "Laura", "Cynthia", "Kathleen", "Amy", "Angela"
};

const QStringList LAST_NAMES = {
    "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis",
    "Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas",
    "Taylor", "Moore", "Jackson", "Martin", "Lee", "Perez", "Thompson", "White",
    "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson", "Walker", "Young",
    "Allen", "King", "Wright", "Scott", "Torres", "Nguyen", "Hill", "Flores"
};

const QStringList STREET_NAMES = {
    "Maple", "Oak", "Cedar", "Elm", "Pine", "Washington", "Lincoln", "Jefferson",
    "Sunset", "Highland", "Willow", "Birch", "Meadow", "Ridge", "Church", "Main",
    "Franklin", "Chestnut", "Spring", "Lakeview"
};

const QStringList STREET_TYPES = {
    "Street", "Avenue", "Road", "Boulevard", "Lane", "Drive", "Way", "Court"
};

const QStringList CITIES = {
    "Springfield", "Riverside", "Fairview", "Georgetown", "Clinton", "Salem",
    "Madison", "Arlington", "Ashland", "Burlington", "Franklin", "Greenville",
    "Manchester", "Oxford", "Winchester"
};

const int MIN_AGE_YEARS = 18;
const int MAX_AGE_YEARS = 80;

const int MIN_HOUSE_NUMBER = 1;
const int MAX_HOUSE_NUMBER = 9999;

}

QString RandomPersona::randomGender()
{
    return QRandomGenerator::global()->bounded(2) == 0 ? "Male" : "Female";
}

QString RandomPersona::randomFirstName(const QString &gender)
{
    const QStringList &pool = gender == "Male" ? MALE_FIRST_NAMES : FEMALE_FIRST_NAMES;
    return pool.at(QRandomGenerator::global()->bounded(pool.size()));
}

QString RandomPersona::randomLastName()
{
    return LAST_NAMES.at(QRandomGenerator::global()->bounded(LAST_NAMES.size()));
}

QDate RandomPersona::randomBirthday()
{
    const QDate today = QDate::currentDate();
    const QDate earliest = today.addYears(-MAX_AGE_YEARS);
    const QDate latest = today.addYears(-MIN_AGE_YEARS);

    const qint64 dayRange = earliest.daysTo(latest);
    return earliest.addDays(QRandomGenerator::global()->bounded(dayRange + 1));
}

QString RandomPersona::randomAddress()
{
    const int houseNumber = MIN_HOUSE_NUMBER
        + QRandomGenerator::global()->bounded(MAX_HOUSE_NUMBER - MIN_HOUSE_NUMBER + 1);
    const QString &streetName = STREET_NAMES.at(QRandomGenerator::global()->bounded(STREET_NAMES.size()));
    const QString &streetType = STREET_TYPES.at(QRandomGenerator::global()->bounded(STREET_TYPES.size()));
    const QString &city = CITIES.at(QRandomGenerator::global()->bounded(CITIES.size()));
    const int postalCode = QRandomGenerator::global()->bounded(10000, 100000);

    return QString("%1 %2 %3, %4, %5")
        .arg(QString::number(houseNumber), streetName, streetType, city, QString::number(postalCode));
}

PersonaData RandomPersona::generate(const QStringList &countries)
{
    PersonaData persona;

    persona.gender = randomGender();
    persona.firstName = randomFirstName(persona.gender);
    persona.lastName = randomLastName();
    persona.birthday = randomBirthday();
    persona.address = randomAddress();

    if (!countries.isEmpty())
        persona.country = countries.at(QRandomGenerator::global()->bounded(countries.size()));

    return persona;
}
