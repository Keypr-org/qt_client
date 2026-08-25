#include "unlockvaultmodal.h"
#include "ui_unlockvaultmodal.h"

unlockvaultmodal::unlockvaultmodal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::unlockvaultmodal)
{
    ui->setupUi(this);
    ui->passwordInput->setLabelText("MASTER PASSWORD");
}

unlockvaultmodal::~unlockvaultmodal()
{
    delete ui;
}
