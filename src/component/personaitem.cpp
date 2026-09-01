#include "personaitem.h"
#include "ui_personaitem.h"

#include <QPainter>
#include <QStyleOption>

PersonaItem::PersonaItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonaItem)
{
    ui->setupUi(this);

    connect(ui->modifyButton, &QPushButton::clicked, this, [this](){
        emit modifyRequested(m_persona);
    });

    connect(ui->deleteButton, &QPushButton::clicked, this, [this](){
        emit deleteRequested(m_persona.id);
    });
}

PersonaItem::~PersonaItem()
{
    delete ui;
}

void PersonaItem::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

const VaultBridge::PersonaSummary &PersonaItem::persona() const
{
    return m_persona;
}

void PersonaItem::setPersona(const VaultBridge::PersonaSummary &persona)
{
    m_persona = persona;

    ui->name->setText(persona.firstName + " " + persona.lastName);

    QString initials;
    if (!persona.firstName.isEmpty()) {
        initials += persona.firstName.at(0).toUpper();
    }
    if (!persona.lastName.isEmpty()) {
        initials += persona.lastName.at(0).toUpper();
    }
    ui->label->setText(initials);

    ui->birthdayValue->setText(persona.dateOfBirth.toString("MMM d, yyyy"));
    ui->addressValue->setText(persona.address);
}
