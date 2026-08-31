#ifndef CREDITCARDENTRY_H
#define CREDITCARDENTRY_H

#include <QWidget>
#include <memory>

#include "model/creditcardentrydata.h"

namespace Ui {
class CreditCardEntry;
}

class CreditCardEntry : public QWidget
{
    Q_OBJECT

public:
    explicit CreditCardEntry(QWidget *parent = nullptr);
    ~CreditCardEntry();

    void setEntry(const std::shared_ptr<CreditCardEntryData> &entry);

signals:
    void deleteRequested(QString id);
    void entryUpdated(QString id);

private:
    Ui::CreditCardEntry *ui;
    std::shared_ptr<CreditCardEntryData> m_entry;
};

#endif // CREDITCARDENTRY_H
