#include "creditcardentry.h"
#include "ui_creditcardentry.h"

#include "component/notificationtooltip.h"

#include "../../utils/qtypes/QCreditCard.h"

CreditCardEntry::CreditCardEntry(QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditCardEntry)
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

        connect(ui->deleteButton, &QPushButton::clicked, this, [this]()
                { emit deleteRequested(m_entryId); });

        connect(ui->applyButton, &QPushButton::clicked, this, [this]()
                {
        if (ui->cardNumberInput->cardNumber().isEmpty() || ui->nameInput->text().isEmpty()) {
            NotificationTooltip::showErrorToast(this, "Please enter a card number and cardholder name.");
            return;
        }

        emit entrySaveRequested(m_entryId, ui->nameInput->text(), ui->cardNumberInput->cardNumber(),
                                 ui->expiresInput->text(), ui->cvvInput->text(), ui->notesInput->text()); });
}

CreditCardEntry::~CreditCardEntry()
{
        delete ui;
}

void CreditCardEntry::setEntry(const QEntry &entry)
{
        m_entryId = entry.getId();

        auto *creditCardEntry = dynamic_cast<const QCreditCard *>(&entry);
        const QString cardLabel = creditCardEntry->getCardNumber().length() >= 4
                                      ? "•••• " + creditCardEntry->getCardNumber().right(4)
                                      : "•••• " + creditCardEntry->getCardNumber();

        ui->titleLabel->setText(cardLabel);
        ui->nameHeader->setText(creditCardEntry->getCardHolderName());

        ui->cardNumberInput->setCardNumber(creditCardEntry->getCardNumber());
        ui->nameInput->setText(creditCardEntry->getCardHolderName());
        ui->expiresInput->setText(creditCardEntry->getExpiration());
        ui->cvvInput->setText(creditCardEntry->getSecurityCode());
        ui->notesInput->setText(creditCardEntry->getNotes());

        ui->card->setCardNumber(creditCardEntry->getCardNumber());
        ui->card->setOwnerName(creditCardEntry->getCardHolderName());
        ui->card->setExpiration(creditCardEntry->getExpiration());
}
