#include "textinput.h"
#include "ui_textinput.h"

TextInput::TextInput(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextInput)
{
    ui->setupUi(this);
}

TextInput::~TextInput()
{
    delete ui;
}


void TextInput::setLabelText(const QString &text)
{
    ui->textLabel->setText(text);
}

void TextInput::setInputPlaceholder(const QString &text)
{
    ui->textEdit->setPlaceholderText(text);
}

void TextInput::setText(const QString &text)
{
    ui->textEdit->setPlainText(text);
}

QString TextInput::text() const
{
    return ui->textEdit->toPlainText();
}