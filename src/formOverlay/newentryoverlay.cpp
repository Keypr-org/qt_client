#include "newentryoverlay.h"
#include "ui_newentryoverlay.h"
#include "utils/hoverwidget.h"

#include <QStyle>
#include <QLabel>

NewEntryOverlay::NewEntryOverlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewEntryOverlay)
{
    ui->setupUi(this);

    setupEntry(ui->wifiEntry, ui->wifiIcon,
               ":/icons/icons/icon-wifi.png",
               ":/icons/icons/icon-wifi-purple.png",
               [this]() { emit newWifiEntryRequested(); });

    setupEntry(ui->ccEntry, ui->ccIcon,
               ":/icons/icons/icon-credit-card.png",
               ":/icons/icons/icon-credit-card-purple.png",
               [this]() { emit newCreditCardEntryRequested(); });

    setupEntry(ui->webEntry, ui->webIcon,
               ":/icons/icons/icon-globe.png",
               ":/icons/icons/icon-globe-purple.png",
               [this]() { emit newWebsiteCredentialsEntryRequested(); });

    connect(ui->cancelButton, &QPushButton::clicked, this, [this]() {
        hide();
    });
}

void NewEntryOverlay::setupEntry(HoverWidget *entry, QLabel *icon,
                                 const QString &defaultIcon,
                                 const QString &hoverIcon,
                                 std::function<void()> signalEmitter)
{
    // Hover enter
    connect(entry, &HoverWidget::hoverEntered, this, [icon, hoverIcon]() {
        icon->setProperty("hovered", true);
        icon->setPixmap(QPixmap(hoverIcon));
        icon->style()->unpolish(icon);
        icon->style()->polish(icon);
    });

    // Hover leave
    connect(entry, &HoverWidget::hoverLeft, this, [icon, defaultIcon]() {
        icon->setProperty("hovered", false);
        icon->setPixmap(QPixmap(defaultIcon));
        icon->style()->unpolish(icon);
        icon->style()->polish(icon);
    });

    // Click
    connect(entry, &HoverWidget::clicked, this, [signalEmitter]() {
        signalEmitter();
    });
}

NewEntryOverlay::~NewEntryOverlay()
{
    delete ui;
}