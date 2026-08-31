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
    explicit WifiEntry(QWidget *parent = nullptr);
    ~WifiEntry();

    void setEntry(const std::shared_ptr<WifiEntryData> &entry);

signals:
    void deleteRequested(QString id);
    void entryUpdated(QString id);

private:
    Ui::WifiEntry *ui;
    std::shared_ptr<WifiEntryData> m_entry;
};

#endif // WIFIENTRY_H
