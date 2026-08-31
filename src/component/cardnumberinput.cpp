#include "cardnumberinput.h"
#include "ui_cardnumberinput.h"
#include "utils/cardnumberutils.h"

CardNumberInput::CardNumberInput(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CardNumberInput),
    isVisible(false),
    inputText()
{
    ui->setupUi(this);

    ui->input->installEventFilter(this);
}

CardNumberInput::~CardNumberInput()
{
    delete ui;
}

void CardNumberInput::on_toggleButton_clicked()
{
    if (isVisible) {
        inputText = ui->input->text();
        ui->input->setText(CardNumberUtils::mask(inputText));
        isVisible = false;
    }
    else {
        ui->input->setText(inputText);
        isVisible = true;
    }
    ui->input->clearFocus();
    emit visibilityButtonClicked(isVisible);
}


bool CardNumberInput::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->input)
    {
        if (event->type() == QEvent::FocusIn && !isVisible)
        {
            ui->input->setText(inputText);
            isVisible = true;
        }
        else if (event->type() == QEvent::FocusOut && isVisible)
        {
            inputText = ui->input->text();
            ui->input->setText(CardNumberUtils::mask(inputText));
            isVisible = false;
            emit visibilityButtonClicked(isVisible);
        }

    }

    return QWidget::eventFilter(obj, event);
}
void CardNumberInput::on_input_textEdited(const QString &cardNumber)
{
    emit cardNumberChanged(cardNumber);
}

void CardNumberInput::setCardNumber(const QString &cardNumber)
{
    inputText = cardNumber;
    isVisible = false;
    ui->input->setText(CardNumberUtils::mask(inputText));
    emit cardNumberChanged(inputText);
}

QString CardNumberInput::cardNumber() const
{
    return inputText;
}

