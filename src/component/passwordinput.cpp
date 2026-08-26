#include "passwordinput.h"
#include "ui_passwordinput.h"

PasswordInput::PasswordInput(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordInput)
{
    ui->setupUi(this);
}

PasswordInput::~PasswordInput()
{
    delete ui;
}

void PasswordInput::on_toggleButton_clicked()
{
    if(ui->input->echoMode() == QLineEdit::Password)
        ui->input->setEchoMode(QLineEdit::Normal);
    else
        ui->input->setEchoMode(QLineEdit::Password);
}

void PasswordInput::setLabelText(const QString &text)
{
    ui->passwordLabel->setText(text);
}

