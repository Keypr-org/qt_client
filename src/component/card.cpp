#include "card.h"
#include "ui_card.h"
#include "utils/cardnumberutils.h"

Card::Card(
    QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Card),
    cardNumber(),
    isVisible(false)
{
    ui->setupUi(this);
}

Card::~Card()
{
    delete ui;
}


void Card::setCardNumber(const QString &cardNumber) {
    this->cardNumber = cardNumber;
    updateCardNumberLabel();
}

void Card::setOwnerName(const QString &ownerName) {
    ui->nameLabel->setText(ownerName);
}

void Card::setExpiration(const QString &expiration) {
    ui->expiresLabel->setText(expiration);
}

void Card::setVisibility(bool visibility) {
    this->isVisible = visibility;
    updateCardNumberLabel();
}

void Card::updateCardNumberLabel() {
    if (this->isVisible)
        ui->cardNumberLabel->setText(CardNumberUtils::format(cardNumber));
    else
        ui->cardNumberLabel->setText(CardNumberUtils::mask(cardNumber));
}
