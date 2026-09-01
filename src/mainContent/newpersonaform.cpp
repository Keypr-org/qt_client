#include "newpersonaform.h"
#include "ui_newpersonaform.h"

#include "component/notificationtooltip.h"
#include "utils/randompersona.h"

#include <QUuid>

NewPersonaForm::NewPersonaForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewPersonaForm)
{
    ui->setupUi(this);

    QList<QLocale> allLocales = QLocale::matchingLocales(
        QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

    QSet<QString> countries;
    for (const QLocale &locale : allLocales) {
        countries.insert(QLocale::territoryToString(locale.territory()));
    }

    QStringList sortedCountries = countries.values();
    sortedCountries.sort();

    ui->countrySelect->addItem("Select Country", "");
    ui->countrySelect->addItems(sortedCountries);

    ui->firstName->setLabelText("FIRST NAME");
    ui->firstName->setInputPlaceholder("e.g. Taylor");

    ui->lastName->setLabelText("LAST NAME");
    ui->lastName->setInputPlaceholder("e.g. Brooks");

    ui->addressInput->setLabelText("PHYSICAL ADDRESS");
    ui->addressInput->setInputPlaceholder("e.g. 221B Baker Street, London, UK");

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        clearForm();
        emit cancelSignal();
    });

    connect(ui->generatePersonaButton, &QPushButton::clicked, this, [this, sortedCountries](){
        PersonaData persona = RandomPersona::generate(sortedCountries);

        ui->firstName->setText(persona.firstName);
        ui->lastName->setText(persona.lastName);
        ui->dateSelect->setDate(persona.birthday);
        ui->genderSelect->setCurrentText(persona.gender);
        ui->countrySelect->setCurrentText(persona.country);
        ui->addressInput->setText(persona.address);

        emit generatePersonaSignal();
    });

    connect(ui->savePersonaButton, &QPushButton::clicked, this, [this](){
        if (ui->firstName->text().isEmpty() || ui->lastName->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a first and last name.");
            return;
        }

        PersonaData persona;
        persona.id = QUuid::createUuid().toString();
        persona.firstName = ui->firstName->text();
        persona.lastName = ui->lastName->text();
        persona.birthday = ui->dateSelect->date();
        persona.gender = ui->genderSelect->currentIndex() >= 0 ? ui->genderSelect->currentText() : QString();
        persona.country = ui->countrySelect->currentIndex() > 0 ? ui->countrySelect->currentText() : QString();
        persona.address = ui->addressInput->text();

        emit usePersonaSignal(persona);
        clearForm();
    });
}

void NewPersonaForm::clearForm()
{
    ui->firstName->setText("");
    ui->lastName->setText("");
    ui->addressInput->setText("");
    ui->dateSelect->setDate(QDate::currentDate());
    ui->genderSelect->setCurrentIndex(-1);
    ui->countrySelect->setCurrentIndex(0);
}

NewPersonaForm::~NewPersonaForm()
{
    delete ui;
}
