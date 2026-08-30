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
    explicit WifiForm(QWidget *parent = nullptr);
    ~WifiForm();

    void clearForm();

signals:
    void cancelRequested();
    void createNewWifiEntry(std::shared_ptr<WifiEntryData> entry);

private:
    Ui::WifiForm *ui;
};

#endif // WIFIFORM_H
