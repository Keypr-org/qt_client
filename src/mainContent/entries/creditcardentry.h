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

private:
    Ui::CreditCardEntry *ui;
    QString m_entryId;
};

#endif // CREDITCARDENTRY_H
