#include "personadisplay.h"
#include "ui_personadisplay.h"

PersonaDisplay::PersonaDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonaDisplay)
{
    ui->setupUi(this);
}

PersonaDisplay::~PersonaDisplay()
{
    delete ui;
}
