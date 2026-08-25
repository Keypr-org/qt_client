#ifndef NOVAULTSELECTED_H
#define NOVAULTSELECTED_H

#include <QWidget>

namespace Ui {
class NoVaultSelected;
}

class NoVaultSelected : public QWidget
{
    Q_OBJECT

public:
    explicit NoVaultSelected(QWidget *parent = nullptr);
    ~NoVaultSelected();

private:
    Ui::NoVaultSelected *ui;
};

#endif // NOVAULTSELECTED_H
