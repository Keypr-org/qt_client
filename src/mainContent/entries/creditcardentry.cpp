#include "creditcardentry.h"
#include "ui_creditcardentry.h"

#include "component/notificationtooltip.h"

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
        if (m_entryId.isEmpty()) {
            return;
        }
        emit deleteRequested(m_entryId);
    });

    connect(ui->applyButton, &QPushButton::clicked, this, [this](){
        if (m_entryId.isEmpty()) {
            return;
        }

        if (ui->cardNumberInput->cardNumber().isEmpty() || ui->nameInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a card number and cardholder name.");
            return;
        }

        emit entrySaveRequested(m_entryId, ui->nameInput->text(), ui->cardNumberInput->cardNumber(),
                                 ui->expiresInput->text(), ui->cvvInput->text(), ui->notesInput->text());
    });
}

CreditCardEntry::~CreditCardEntry()
{
    delete ui;
}

void CreditCardEntry::setEntry(const VaultBridge::EntrySummary &entry)
{
    m_entryId = entry.id;

    const QString cardLabel = entry.cardNumber.length() >= 4
        ? "•••• " + entry.cardNumber.right(4)
        : "•••• " + entry.cardNumber;

    ui->titleLabel->setText(cardLabel);
    ui->nameHeader->setText(entry.cardHolderName);

    ui->cardNumberInput->setCardNumber(entry.cardNumber);
    ui->nameInput->setText(entry.cardHolderName);
    ui->expiresInput->setText(entry.expiration);
    ui->cvvInput->setText(entry.securityCode);
    ui->notesInput->setText(entry.notes);

    ui->card->setCardNumber(entry.cardNumber);
    ui->card->setOwnerName(entry.cardHolderName);
    ui->card->setExpiration(entry.expiration);
}
