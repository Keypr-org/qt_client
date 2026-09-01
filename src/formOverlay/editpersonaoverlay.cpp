#include "editpersonaoverlay.h"
#include "ui_editpersonaoverlay.h"

#include "component/notificationtooltip.h"

EditPersonaOverlay::EditPersonaOverlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditPersonaOverlay)
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

    hide();

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        emit cancelled();
        hide();
    });

    connect(ui->saveChangesButton, &QPushButton::clicked, this, [this](){
        if (ui->firstName->text().isEmpty() || ui->lastName->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a first and last name.");
            return;
        }

        emit personaModified(m_personaId, ui->firstName->text(), ui->lastName->text(),
                              ui->dateSelect->date(), ui->addressInput->text(), ui->phoneInput->text());
        hide();
    });
}

EditPersonaOverlay::~EditPersonaOverlay()
{
    delete ui;
}

void EditPersonaOverlay::setPersona(const VaultBridge::PersonaSummary &persona)
{
    m_personaId = persona.id;

    ui->firstName->setText(persona.firstName);
    ui->lastName->setText(persona.lastName);
    ui->addressInput->setText(persona.address);
    ui->phoneInput->setText(persona.phone);
    ui->dateSelect->setDate(persona.dateOfBirth.isValid() ? persona.dateOfBirth : QDate::currentDate());
}

void EditPersonaOverlay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (parentWidget()) {
        setGeometry(parentWidget()->rect());
    }
}
