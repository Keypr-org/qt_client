#include "creditcardentry.h"
#include "ui_creditcardentry.h"

#include <QDateTime>

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
        if (!m_entry) {
            return;
        }
        emit deleteRequested(m_entry->id);
    });

    connect(ui->applyButton, &QPushButton::clicked, this, [this](){
        if (!m_entry) {
            return;
        }

        const QString cardNumber = ui->cardNumberInput->cardNumber();
        const QString ownerName = ui->nameInput->text();
        const QString cardLabel = cardNumber.length() >= 4
            ? "•••• " + cardNumber.right(4)
            : "•••• " + cardNumber;

        m_entry->cardNumber = cardNumber;
        m_entry->ownerName = ownerName;
        m_entry->expiration = ui->expiresInput->text();
        m_entry->cvv = ui->cvvInput->text();
        m_entry->notes = ui->notesInput->text();
        m_entry->cardLabel = cardLabel;
        m_entry->primaryInfo = cardLabel;
        m_entry->secondaryInfo = ownerName;
        m_entry->lastUpdated = QDateTime::currentDateTime();

        ui->titleLabel->setText(cardLabel);
        ui->nameHeader->setText(ownerName);

        emit entryUpdated(m_entry->id);
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

    m_entry = entry;

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
