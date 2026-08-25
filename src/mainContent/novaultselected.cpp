#include "novaultselected.h"
#include "ui_novaultselected.h"

NoVaultSelected::NoVaultSelected(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NoVaultSelected)
{
    ui->setupUi(this);
}

NoVaultSelected::~NoVaultSelected()
{
    delete ui;
}
