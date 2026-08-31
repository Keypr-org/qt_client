#ifndef WIFIFORM_H
#define WIFIFORM_H

#include <QWidget>
#include <memory>

#include "model/wifientrydata.h"

namespace Ui {
class WifiForm;
}

class WifiForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the wifi entry creation form.
     * @param parent Parent widget, or nullptr.
     */
    explicit WifiForm(QWidget *parent = nullptr);

    /**
     * @brief Destroys the form and its UI.
     */
    ~WifiForm();

    /**
     * @brief Resets all form fields to their empty/default state.
     */
    void clearForm();

signals:
    /**
     * @brief Emitted when the user cancels creating a new wifi entry.
     */
    void cancelRequested();

    /**
     * @brief Emitted when the user submits the form with a valid new wifi entry.
     * @param entry Wifi entry data built from the form fields.
     */
    void createNewWifiEntry(std::shared_ptr<WifiEntryData> entry);

private:
    Ui::WifiForm *ui;
};

#endif // WIFIFORM_H
