#include "unlockvaultmodal.h"
#include "ui_unlockvaultmodal.h"

unlockvaultmodal::unlockvaultmodal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::unlockvaultmodal)
{
    ui->setupUi(this);
}

unlockvaultmodal::~unlockvaultmodal()
{
    delete ui;
}
