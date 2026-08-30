#ifndef CREDITCARDFORM_H
#define CREDITCARDFORM_H

#include <QWidget>
#include <memory>

#include "model/creditcardentrydata.h"

namespace Ui {
class CreditCardForm;
}

class CreditCardForm : public QWidget
{
    Q_OBJECT

public:
    explicit CreditCardForm(QWidget *parent = nullptr);
    ~CreditCardForm();

    void clearForm();

signals:
    void cancelRequested();
    void createCreditCardEntry(std::shared_ptr<CreditCardEntryData> entry);

private:
    Ui::CreditCardForm *ui;
};

#endif // CREDITCARDFORM_H
