#ifndef NOTIFICATIONTOOLTIP_H
#define NOTIFICATIONTOOLTIP_H

#include <QWidget>

namespace Ui {
class NotificationTooltip;
}

class NotificationTooltip : public QWidget
{
    Q_OBJECT

public:
    enum class Type {
        Error,
        Success
    };

    /**
     * @brief Constructs the notification tooltip widget.
     * @param parent Parent widget.
     */
    explicit NotificationTooltip(QWidget *parent = nullptr);
    /**
     * @brief Destroys the widget and its UI.
     */
    ~NotificationTooltip();

    /**
     * @brief Displays a message styled according to the given type.
     * @param text The message text.
     * @param type The notification style (error or success).
     */
    void showMessage(const QString &text, Type type);
    /**
     * @brief Displays a message styled as an error.
     * @param text The message text.
     */
    void showError(const QString &text);
    /**
     * @brief Displays a message styled as a success.
     * @param text The message text.
     */
    void showSuccess(const QString &text);
    /**
     * @brief Hides the currently displayed message.
     */
    void hideMessage();

    /**
     * @brief Shows a floating toast that overlays `parent`'s window, auto-dismissing after durationMs.
     * @param parent The widget whose window the toast is anchored to.
     * @param text The message text.
     * @param type The notification style (error or success).
     * @param durationMs Time in milliseconds before the toast auto-dismisses.
     */
    static void showToast(QWidget *parent, const QString &text, Type type, int durationMs = 4000);
    /**
     * @brief Convenience wrapper around showToast() for a success-styled toast.
     * @param parent The widget whose window the toast is anchored to.
     * @param text The message text.
     * @param durationMs Time in milliseconds before the toast auto-dismisses.
     */
    static void showSuccessToast(QWidget *parent, const QString &text, int durationMs = 4000);
    /**
     * @brief Convenience wrapper around showToast() for an error-styled toast.
     * @param parent The widget whose window the toast is anchored to.
     * @param text The message text.
     * @param durationMs Time in milliseconds before the toast auto-dismisses.
     */
    static void showErrorToast(QWidget *parent, const QString &text, int durationMs = 4000);

private:
    Ui::NotificationTooltip *ui;
};

#endif // NOTIFICATIONTOOLTIP_H
