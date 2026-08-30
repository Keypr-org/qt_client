#include "personadisplay.h"
#include "ui_personadisplay.h"

PersonaDisplay::PersonaDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonaDisplay)
{
    ui->setupUi(this);

    connect(ui->createPersonaButton, &QPushButton::clicked, this, [this](){
        emit createPersona();
    });
}

PersonaDisplay::~PersonaDisplay()
{
    delete ui;
}
