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
    explicit CreditCardForm(QWidget *parent = nullptr);
    ~CreditCardForm();

private:
    Ui::CreditCardForm *ui;
};

#endif // CREDITCARDFORM_H
