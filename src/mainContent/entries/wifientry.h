#ifndef WIFIENTRY_H
#define WIFIENTRY_H

#include <QWidget>

#include "vaultbridge.h"

namespace Ui {
class WifiEntry;
}

class WifiEntry : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Builds the wifi entry detail widget.
     * @param parent Parent widget.
     */
    explicit WifiEntry(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and releases its UI resources.
     */
    ~WifiEntry();

    /**
     * @brief Populates the widget's fields from the given wifi entry.
     * @param entry Wifi entry data to display and edit.
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
    void entrySaveRequested(QString id, QString networkName, QString password, QString notes);

private:
    Ui::WifiEntry *ui;
    QString m_entryId;
};

#endif // WIFIENTRY_H
