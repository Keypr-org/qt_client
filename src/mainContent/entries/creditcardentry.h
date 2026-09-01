#ifndef CREDITCARDENTRY_H
#define CREDITCARDENTRY_H

#include <QWidget>

#include "vaultbridge.h"

namespace Ui {
class CreditCardEntry;
}

class CreditCardEntry : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Builds the credit card entry detail widget.
     * @param parent Parent widget.
     */
    explicit CreditCardEntry(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and releases its UI resources.
     */
    ~CreditCardEntry();

    /**
     * @brief Populates the widget's fields from the given credit card entry.
     * @param entry Credit card entry data to display and edit.
     */
    void setEntry(const VaultBridge::EntrySummary &entry);

signals:
    /**
     * @brief Emitted when the user requests deletion of the currently displayed entry.
     * @param id Identifier of the entry to delete.
     */
    void deleteRequested(QString id);

    /**
     * @brief Emitted when the user applies edits to the currently displayed entry's fields.
     */
    void entrySaveRequested(QString id, QString cardHolderName, QString cardNumber,
                             QString expiration, QString securityCode, QString notes);

private:
    Ui::CreditCardEntry *ui;
    QString m_entryId;
};

#endif // CREDITCARDENTRY_H
