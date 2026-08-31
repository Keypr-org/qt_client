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
    void setEntry(const std::shared_ptr<CreditCardEntryData> &entry);

signals:
    /**
     * @brief Emitted when the user requests deletion of the currently displayed entry.
     * @param id Identifier of the entry to delete.
     */
    void deleteRequested(QString id);

    /**
     * @brief Emitted after the currently displayed entry has been edited and applied.
     * @param id Identifier of the updated entry.
     */
    void entryUpdated(QString id);

private:
    Ui::CreditCardEntry *ui;
    std::shared_ptr<CreditCardEntryData> m_entry;
};

#endif // CREDITCARDENTRY_H
