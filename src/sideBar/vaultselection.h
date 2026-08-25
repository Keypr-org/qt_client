#ifndef VAULTSELECTION_H
#define VAULTSELECTION_H

#include <QWidget>

namespace Ui {
class VaultSelection;
}

class VaultSelection : public QWidget
{
    Q_OBJECT

public:
    explicit VaultSelection(QWidget *parent = nullptr);
    ~VaultSelection();

private:
    Ui::VaultSelection *ui;
    void updateArrowIcon(bool expanded);
};

#endif // VAULTSELECTION_H
