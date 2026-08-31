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

    explicit NotificationTooltip(QWidget *parent = nullptr);
    ~NotificationTooltip();

    void showMessage(const QString &text, Type type);
    void showError(const QString &text);
    void showSuccess(const QString &text);
    void hideMessage();

    // Floating toast that overlays `parent`'s window, auto-dismissing after durationMs.
    static void showToast(QWidget *parent, const QString &text, Type type, int durationMs = 4000);
    static void showSuccessToast(QWidget *parent, const QString &text, int durationMs = 4000);
    static void showErrorToast(QWidget *parent, const QString &text, int durationMs = 4000);

private:
    Ui::NotificationTooltip *ui;
};

#endif // NOTIFICATIONTOOLTIP_H
