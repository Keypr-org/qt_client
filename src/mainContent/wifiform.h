#ifndef WIFIFORM_H
#define WIFIFORM_H

#include <QWidget>

namespace Ui {
class WifiForm;
}

class WifiForm : public QWidget
{
    Q_OBJECT

public:
    explicit WifiForm(QWidget *parent = nullptr);
    ~WifiForm();

private:
    Ui::WifiForm *ui;
};

#endif // WIFIFORM_H
