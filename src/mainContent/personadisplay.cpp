#include "personadisplay.h"
#include "ui_personadisplay.h"
#include "component/personaitem.h"

PersonaDisplay::PersonaDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonaDisplay)
{
    ui->setupUi(this);

    ui->personasScrollArea->viewport()->setAutoFillBackground(true);
    ui->personasScrollArea->viewport()->setStyleSheet("background-color: #0F0F0A;");

    connect(ui->createPersonaButton, &QPushButton::clicked, this, [this](){
        emit createPersona();
    });
}

PersonaDisplay::~PersonaDisplay()
{
    delete ui;
}

void PersonaDisplay::addPersona(const PersonaData &persona)
{
    auto *item = new PersonaItem(ui->personasScrollContent);
    item->setPersona(persona);

    const int row = m_personaCount / GRID_COLUMNS;
    const int column = m_personaCount % GRID_COLUMNS;
    ui->gridLayout->addWidget(item, row, column);

    ++m_personaCount;
}
