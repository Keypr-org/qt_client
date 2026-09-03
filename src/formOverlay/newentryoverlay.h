#ifndef NEWENTRYOVERLAY_H
#define NEWENTRYOVERLAY_H

#include <QWidget>
#include <QLabel>
#include "utils/hoverwidget.h"

namespace Ui {
class NewEntryOverlay;
}

class NewEntryOverlay : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the new-entry type selection overlay.
     * @param parent Parent widget.
     */
    explicit NewEntryOverlay(QWidget *parent = nullptr);

    /**
     * @brief Destroys the overlay and releases its UI resources.
     */
    ~NewEntryOverlay();

signals:
    /**
     * @brief Emitted when the user picks the "Wifi" entry type.
     */
    void newWifiEntryRequested();

    /**
     * @brief Emitted when the user picks the "Credit card" entry type.
     */
    void newCreditCardEntryRequested();

    /**
     * @brief Emitted when the user picks the "Website credentials" entry type.
     */
    void newWebsiteCredentialsEntryRequested();

private:
    Ui::NewEntryOverlay *ui;

    /**
     * @brief Wires a selectable entry-type tile's hover icon swap and click-to-signal behavior.
     * @param entry Hoverable widget representing the entry-type tile.
     * @param icon Label showing the tile's icon.
     * @param defaultIcon Resource path of the icon shown when not hovered.
     * @param hoverIcon Resource path of the icon shown while hovered.
     * @param signalEmitter Callback invoked to emit the tile's corresponding "new entry" signal.
     */
    void setupEntry(HoverWidget *entry, QLabel *icon,
                    const QString &defaultIcon,
                    const QString &hoverIcon,
                    std::function<void()> signalEmitter);
};

#endif // NEWENTRYOVERLAY_H
