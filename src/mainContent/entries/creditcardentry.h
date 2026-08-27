#ifndef CREDITCARDENTRY_H
#define CREDITCARDENTRY_H

#include <QWidget>

namespace Ui {
class CreditCardEntry;
}

class CreditCardEntry : public QWidget
{
    Q_OBJECT

public:
    explicit CreditCardEntry(QWidget *parent = nullptr);
    ~CreditCardEntry();

private:
    Ui::CreditCardEntry *ui;
};

#endif // CREDITCARDENTRY_H
