#ifndef CARD_H
#define CARD_H

#include <QWidget>

namespace Ui {
class Card;
}

class Card : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the credit card visual widget.
     * @param parent Parent widget.
     */
    explicit Card(
        QWidget *parent = nullptr);
    /**
     * @brief Destroys the widget and its UI.
     */
    ~Card();

    /**
     * @brief Sets the card number to display, applying masking/formatting as needed.
     * @param cardNumber The raw card number.
     */
    void setCardNumber(const QString &cardNumber);
    /**
     * @brief Sets the displayed cardholder name.
     * @param ownerName The name to display.
     */
    void setOwnerName(const QString &ownerName);
    /**
     * @brief Sets the displayed expiration date.
     * @param expiration The expiration date text.
     */
    void setExpiration(const QString &expiration);
    /**
     * @brief Toggles whether the full card number or a masked version is shown.
     * @param visibility True to show the full number, false to mask it.
     */
    void setVisibility(bool visibility);

private:
    Ui::Card *ui;


    QString cardNumber;
    bool isVisible;

    /**
     * @brief Refreshes the card number label to reflect the current number and visibility state.
     */
    void updateCardNumberLabel();
};

#endif // CARD_H
