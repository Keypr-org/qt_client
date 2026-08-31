#include "editpersonaoverlay.h"
#include "ui_editpersonaoverlay.h"

#include <QLocale>
#include <QSet>
#include <QStringList>

EditPersonaOverlay::EditPersonaOverlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditPersonaOverlay)
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

    hide();

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        emit cancelled();
        hide();
    });

    connect(ui->saveChangesButton, &QPushButton::clicked, this, [this](){
        PersonaData persona;
        persona.id = m_personaId;
        persona.firstName = ui->firstName->text();
        persona.lastName = ui->lastName->text();
        persona.birthday = ui->dateSelect->date();
        persona.gender = ui->genderSelect->currentIndex() >= 0 ? ui->genderSelect->currentText() : QString();
        persona.country = ui->countrySelect->currentIndex() > 0 ? ui->countrySelect->currentText() : QString();

        emit personaModified(persona);
        hide();
    });
}

EditPersonaOverlay::~EditPersonaOverlay()
{
    delete ui;
}

void EditPersonaOverlay::setPersona(const PersonaData &persona)
{
    m_personaId = persona.id;

    ui->firstName->setText(persona.firstName);
    ui->lastName->setText(persona.lastName);
    ui->dateSelect->setDate(persona.birthday.isValid() ? persona.birthday : QDate::currentDate());

    const int genderIndex = ui->genderSelect->findText(persona.gender);
    ui->genderSelect->setCurrentIndex(genderIndex);

    const int countryIndex = ui->countrySelect->findText(persona.country);
    ui->countrySelect->setCurrentIndex(countryIndex >= 0 ? countryIndex : 0);
}

void EditPersonaOverlay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (parentWidget()) {
        setGeometry(parentWidget()->rect());
    }
}
