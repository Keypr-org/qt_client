#ifndef CARDNUMBERINPUT_H
#define CARDNUMBERINPUT_H

#include <QWidget>

namespace Ui {
class CardNumberInput;
}

class CardNumberInput : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the masked card-number input field.
     * @param parent Parent widget.
     */
    explicit CardNumberInput(QWidget *parent = nullptr);
    /**
     * @brief Destroys the widget and its UI.
     */
    ~CardNumberInput();

    /**
     * @brief Sets the card number held by the field, formatting/masking it for display.
     * @param cardNumber The raw card number.
     */
    void setCardNumber(const QString &cardNumber);
    /**
     * @brief Returns the current unmasked card number.
     * @return The raw card number.
     */
    QString cardNumber() const;

signals:
    /**
     * @brief Emitted when the show/hide visibility toggle button is clicked.
     * @param visible True if the number is now shown in full, false if masked.
     */
    void visibilityButtonClicked(bool visible);
    /**
     * @brief Emitted whenever the underlying card number value changes.
     * @param cardNumber The new card number.
     */
    void cardNumberChanged(QString cardNumber);

private slots:
    /**
     * @brief Handles clicks on the visibility toggle button, flipping masked/unmasked display.
     */
    void on_toggleButton_clicked();

    /**
     * @brief Handles user edits to the raw input field, reformatting and re-masking as needed.
     * @param arg1 The current raw text of the input field.
     */
    void on_input_textEdited(const QString &arg1);

private:
    Ui::CardNumberInput *ui;

    bool isVisible;
    QString inputText;

    /**
     * @brief Intercepts events on watched objects, used to manage masked-input display behavior.
     * @param obj The object the event originated from.
     * @param event The intercepted event.
     * @return True if the event was handled and should be filtered out, false to let it propagate.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // CARDNUMBERINPUT_H
