#include "creditcardentry.h"
#include "ui_creditcardentry.h"

CreditCardEntry::CreditCardEntry(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreditCardEntry)
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

    connect(ui->deleteButton, &QPushButton::clicked, this, [this](){
        emit deleteRequested(m_entryId);
    });
}

CreditCardEntry::~CreditCardEntry()
{
    delete ui;
}

void CreditCardEntry::setEntry(const std::shared_ptr<CreditCardEntryData> &entry)
{
    if (!entry) {
        return;
    }

    m_entryId = entry->id;

    ui->titleLabel->setText(entry->cardLabel);
    ui->nameHeader->setText(entry->ownerName);

    ui->cardNumberInput->setCardNumber(entry->cardNumber);
    ui->nameInput->setText(entry->ownerName);
    ui->expiresInput->setText(entry->expiration);
    ui->cvvInput->setText(entry->cvv);
    ui->notesInput->setText(entry->notes);

    ui->card->setCardNumber(entry->cardNumber);
    ui->card->setOwnerName(entry->ownerName);
    ui->card->setExpiration(entry->expiration);
}
