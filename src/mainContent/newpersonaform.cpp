#include "newpersonaform.h"
#include "ui_newpersonaform.h"

#include "component/notificationtooltip.h"
#include "utils/randompersona.h"

NewPersonaForm::NewPersonaForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewPersonaForm)
{
    ui->setupUi(this);

    ui->firstName->setLabelText("FIRST NAME");
    ui->firstName->setInputPlaceholder("e.g. Taylor");

    ui->lastName->setLabelText("LAST NAME");
    ui->lastName->setInputPlaceholder("e.g. Brooks");

    ui->addressInput->setLabelText("PHYSICAL ADDRESS");
    ui->addressInput->setInputPlaceholder("e.g. 221B Baker Street, London, UK");

    ui->phoneInput->setLabelText("PHONE");
    ui->phoneInput->setInputPlaceholder("e.g. +1 555-123-4567");

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        clearForm();
        emit cancelSignal();
    });

    connect(ui->generatePersonaButton, &QPushButton::clicked, this, [this](){
        const auto persona = RandomPersona::generate();

        ui->firstName->setText(persona.firstName);
        ui->lastName->setText(persona.lastName);
        ui->dateSelect->setDate(persona.birthday);
        ui->addressInput->setText(persona.address);
        ui->phoneInput->setText(persona.phone);

        emit generatePersonaSignal();
    });

    connect(ui->savePersonaButton, &QPushButton::clicked, this, [this](){
        if (ui->firstName->text().isEmpty() || ui->lastName->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a first and last name.");
            return;
        }

        emit usePersonaSignal(ui->firstName->text(), ui->lastName->text(), ui->dateSelect->date(),
                               ui->addressInput->text(), ui->phoneInput->text());
        clearForm();
    });
}

void NewPersonaForm::clearForm()
{
    ui->firstName->setText("");
    ui->lastName->setText("");
    ui->addressInput->setText("");
    ui->phoneInput->setText("");
    ui->dateSelect->setDate(QDate::currentDate());
}

NewPersonaForm::~NewPersonaForm()
{
    delete ui;
}
