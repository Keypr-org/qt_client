#include "creditcardform.h"
#include "ui_creditcardform.h"

#include "component/notificationtooltip.h"

#include <QDateEdit>

CreditCardForm::CreditCardForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreditCardForm)
{
    ui->setupUi(this);

    connect(ui->cardNumberInput, &CardNumberInput::visibilityButtonClicked,
            ui->card, &Card::setVisibility);

    connect(ui->cardNumberInput, &CardNumberInput::cardNumberChanged,
            ui->card, &Card::setCardNumber);

    connect(ui->nameInput, &LineTextInput::textChanged,
            ui->card, &Card::setOwnerName);

    connect(ui->expiresInput, &QDateEdit::dateChanged, this, [this](const QDate &date){
        ui->card->setExpiration(date.toString("MM/yy"));
    });

    ui->card->setExpiration(ui->expiresInput->date().toString("MM/yy"));

    ui->nameInput->setLabelText("cardholder name");
    ui->cvvInput->setLabelText("cvv");
    ui->notesInput->setLabelText("notes (optional)");
    ui->notesInput->setInputPlaceholder("Enter notes here...");

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        clearForm();
        emit cancelRequested();
    });

    connect(ui->saveButton, &QPushButton::clicked, this, [this](){
        if (ui->cardNumberInput->cardNumber().isEmpty() || ui->nameInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a card number and cardholder name.");
            return;
        }

        emit createCreditCardEntry(
            ui->nameInput->text(),
            ui->cardNumberInput->cardNumber(),
            ui->expiresInput->date().toString("MM/yy"),
            ui->cvvInput->text(),
            ui->notesInput->text());
    });
}

void CreditCardForm::clearForm()
{
    ui->cardNumberInput->setCardNumber("");
    ui->nameInput->setText("");
    ui->expiresInput->setDate(QDate::currentDate());
    ui->cvvInput->setText("");
    ui->notesInput->setText("");

    ui->card->setOwnerName("");
    ui->card->setExpiration(ui->expiresInput->date().toString("MM/yy"));
}

CreditCardForm::~CreditCardForm()
{
    delete ui;
}
