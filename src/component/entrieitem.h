#ifndef ENTRIEITEM_H
#define ENTRIEITEM_H

#include <QWidget>

namespace Ui {
class EntrieItem;
}

class EntrieItem : public QWidget
{
    Q_OBJECT

public:
    enum IconType {
        WebSite,
        Wifi,
        CreditCard
    };


    /**
     * @brief Constructs a list-row widget representing a single vault entry.
     * @param parent Parent widget.
     */
    explicit EntrieItem(QWidget *parent = nullptr);
    /**
     * @brief Destroys the widget and its UI.
     */
    ~EntrieItem();

    /**
     * @brief Sets the entry-type icon shown on the row.
     * @param type The icon to display.
     */
    void setIcon(IconType type);
    /**
     * @brief Sets the row's selected/highlighted visual state.
     * @param selected True to mark the row as selected.
     */
    void setSelected(bool selected);
    /**
     * @brief Sets the row's primary (main) label text.
     * @param text The primary info to display.
     */
    void setPrimaryInfo(const QString &text);
    /**
     * @brief Sets the row's secondary (subtitle) label text.
     * @param text The secondary info to display.
     */
    void setSecondaryInfo(const QString &text);
    /**
     * @brief Sets the row's last-updated label text.
     * @param text The last-update text to display.
     */
    void setLastUpdate(const QString &text);

private:
    Ui::EntrieItem *ui;
    IconType currentIcon;
};

#endif // ENTRIEITEM_H
