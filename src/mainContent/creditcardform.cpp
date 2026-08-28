#include "creditcardform.h"
#include "ui_creditcardform.h"

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

    connect(ui->expiresInput, &LineTextInput::textChanged,
            ui->card, &Card::setExpiration);

    ui->nameInput->setLabelText("cardholder name");
    ui->expiresInput->setLabelText("expiration date");
    ui->cvvInput->setLabelText("cvv");
    ui->notesInput->setLabelText("notes (optional)");
    ui->notesInput->setInputPlaceholder("Enter notes here...");

    connect(ui->cancelButton, &QPushButton::clicked, this, [this](){
        emit cancelRequested();
    });

    connect(ui->saveButton, &QPushButton::clicked, this, [this](){
        emit createCreditCardEntry();
    });
}

CreditCardForm::~CreditCardForm()
{
    delete ui;
}
