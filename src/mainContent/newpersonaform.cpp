#include "newpersonaform.h"
#include "ui_newpersonaform.h"

NewPersonaForm::NewPersonaForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewPersonaForm)
{
    ui->setupUi(this);

    QList<QLocale> allLocales = QLocale::matchingLocales(
        QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

    QSet<QString> countries;
    for (const QLocale &locale : allLocales) {
        countries.insert(QLocale::countryToString(locale.country()));
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
        emit cancelSignal();
    });

    connect(ui->generatePersonaButton, &QPushButton::clicked, this, [this](){
        emit generatePersonaSignal();
    });

    connect(ui->savePersonaButton, &QPushButton::clicked, this, [this](){
        emit usePersonaSignal();
    });
}

NewPersonaForm::~NewPersonaForm()
{
    delete ui;
}
