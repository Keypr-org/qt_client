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
    explicit CardNumberInput(QWidget *parent = nullptr);
    ~CardNumberInput();

    void setCardNumber(const QString &cardNumber);
    QString cardNumber() const;

signals:
    void visibilityButtonClicked(bool visible);
    void cardNumberChanged(QString cardNumber);

private slots:
    void on_toggleButton_clicked();

    void on_input_textEdited(const QString &arg1);

private:
    Ui::CardNumberInput *ui;

    bool isVisible;
    QString inputText;

    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // CARDNUMBERINPUT_H
