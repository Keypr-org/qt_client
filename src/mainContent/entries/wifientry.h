#ifndef WIFIENTRY_H
#define WIFIENTRY_H

#include <QWidget>
#include <memory>

#include "model/wifientrydata.h"

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
    void setEntry(const std::shared_ptr<WifiEntryData> &entry);

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
    Ui::WifiEntry *ui;
    std::shared_ptr<WifiEntryData> m_entry;
};

#endif // WIFIENTRY_H
