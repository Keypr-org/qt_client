#ifndef PASSWORDINPUT_H
#define PASSWORDINPUT_H

#include <QWidget>

namespace Ui {
class PasswordInput;
}

class PasswordInput : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordInput(QWidget *parent = nullptr);
    ~PasswordInput();

private slots:
    void on_toggleButton_clicked();

private:
    Ui::PasswordInput *ui;
};

#endif // PASSWORDINPUT_H
