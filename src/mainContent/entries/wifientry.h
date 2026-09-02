#ifndef WIFIENTRY_H
#define WIFIENTRY_H

#include <QWidget>

#include "../../utils/qtypes/QEntry.h"

namespace Ui
{
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
    void setEntry(const QEntry &entry);

signals:
    /**
     * @brief Emitted when the user requests deletion of the currently displayed entry.
     * @param id Identifier of the entry to delete.
     */
    void deleteRequested(qint64 id);

    /**
     * @brief Emitted when the user applies edits to the currently displayed entry's fields.
     */
    void entrySaveRequested(qint64 id, QString networkName, QString password, QString notes);

private:
    Ui::WifiEntry *ui;
    qint64 m_entryId;
};

#endif // WIFIENTRY_H
