#include "notificationtooltip.h"
#include "ui_notificationtooltip.h"

#include <QPixmap>
#include <QStyle>
#include <QTimer>
#include <algorithm>

NotificationTooltip::NotificationTooltip(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NotificationTooltip)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
    hide();
}

NotificationTooltip::~NotificationTooltip()
{
    delete ui;
}

void NotificationTooltip::showMessage(const QString &text, Type type)
{
    ui->notificationText->setText(text);

    const QString typeValue = (type == Type::Error) ? "error" : "success";
    const QString icon = (type == Type::Error) ? ":/icons/icons/alert-triangle.png"
                                                : ":/icons/icons/icon-check-green.png";

    setProperty("messageType", typeValue);
    style()->unpolish(this);
    style()->polish(this);

    ui->notificationIcon->setPixmap(QPixmap(icon));

    show();
}

void NotificationTooltip::showError(const QString &text)
{
    showMessage(text, Type::Error);
}

void NotificationTooltip::showSuccess(const QString &text)
{
    showMessage(text, Type::Success);
}

void NotificationTooltip::hideMessage()
{
    hide();
}

void NotificationTooltip::showToast(QWidget *parent, const QString &text, Type type, int durationMs)
{
    if (!parent)
        return;

    QWidget *host = parent->window();

    auto *toast = new NotificationTooltip(host);
    toast->setAttribute(Qt::WA_DeleteOnClose);
    toast->showMessage(text, type);

    const int margin = 24;
    toast->adjustSize();
    const int width = std::min(host->width() - 2 * margin, 420);
    toast->setFixedWidth(width);
    toast->move((host->width() - width) / 2, margin);
    toast->raise();
    toast->show();

    QTimer::singleShot(durationMs, toast, [toast]() {
        toast->close();
    });
}

void NotificationTooltip::showSuccessToast(QWidget *parent, const QString &text, int durationMs)
{
    showToast(parent, text, Type::Success, durationMs);
}

void NotificationTooltip::showErrorToast(QWidget *parent, const QString &text, int durationMs)
{
    showToast(parent, text, Type::Error, durationMs);
}
