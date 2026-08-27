#ifndef WIFIENTRY_H
#define WIFIENTRY_H

#include <QWidget>

namespace Ui {
class WifiEntry;
}

class WifiEntry : public QWidget
{
    Q_OBJECT

public:
    explicit WifiEntry(QWidget *parent = nullptr);
    ~WifiEntry();

private:
    Ui::WifiEntry *ui;
};

#endif // WIFIENTRY_H
