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
    explicit NewEntryOverlay(QWidget *parent = nullptr);
    ~NewEntryOverlay();

signals:
    void newWifiEntryRequested();
    void newCreditCardEntryRequested();
    void newWebsiteCredentialsEntryRequested();

private:
    Ui::NewEntryOverlay *ui;

    void setupEntry(HoverWidget *entry, QLabel *icon,
                    const QString &defaultIcon,
                    const QString &hoverIcon,
                    std::function<void()> signalEmitter);
};

#endif // NEWENTRYOVERLAY_H
