#ifndef CREDITCARDFORM_H
#define CREDITCARDFORM_H

#include <QWidget>

namespace Ui {
class CreditCardForm;
}

class CreditCardForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the credit card entry creation form.
     * @param parent Parent widget, or nullptr.
     */
    explicit CreditCardForm(QWidget *parent = nullptr);

    /**
     * @brief Destroys the form and its UI.
     */
    ~CreditCardForm();

    /**
     * @brief Resets all form fields to their empty/default state.
     */
    void clearForm();

signals:
    /**
     * @brief Emitted when the user cancels creating a new credit card entry.
     */
    void cancelRequested();

    /**
     * @brief Emitted when the user submits the form with a valid new credit card entry.
     */
    void createCreditCardEntry(QString cardHolderName, QString cardNumber, QString expiration,
                                QString securityCode, QString notes);

private:
    Ui::CreditCardForm *ui;
};

#endif // CREDITCARDFORM_H
