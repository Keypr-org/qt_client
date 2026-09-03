#include "personaitem.h"
#include "ui_personaitem.h"

#include <QPainter>
#include <QStyleOption>
#include <QDate>

PersonaItem::PersonaItem(QWidget *parent)
    : QWidget(parent), ui(new Ui::PersonaItem)
{
    ui->setupUi(this);

    connect(ui->modifyButton, &QPushButton::clicked, this, [this]()
            { emit modifyRequested(*m_persona.get()); });

    connect(ui->deleteButton, &QPushButton::clicked, this, [this]()
            { emit deleteRequested(m_persona->getId()); });
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

const QPersona PersonaItem::persona() const
{
    return *m_persona.get();
}

void PersonaItem::setPersona(const QPersona &persona)
{
    m_persona = std::make_unique<QPersona>(persona);

    ui->name->setText(persona.getFirstName() + " " + persona.getLastName());

    QString initials;
    if (!persona.getFirstName().isEmpty())
    {
        initials += persona.getFirstName().at(0).toUpper();
    }
    if (!persona.getLastName().isEmpty())
    {
        initials += persona.getLastName().at(0).toUpper();
    }
    ui->label->setText(initials);

    ui->birthdayValue->setText(persona.getDateOfBirth().toString("MMM d, yyyy"));
    ui->addressValue->setText(persona.getAddress());
}
