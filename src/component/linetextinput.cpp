#include "linetextinput.h"
#include "ui_linetextinput.h"

LineTextInput::LineTextInput(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LineTextInput)
{
    ui->setupUi(this);
}

LineTextInput::~LineTextInput()
{
    delete ui;
}

void LineTextInput::setLabelText(const QString &text)
{
    ui->lineTextLabel->setText(text);
}

void LineTextInput::setInputPlaceholder(const QString &text)
{
    ui->input->setPlaceholderText(text);
}