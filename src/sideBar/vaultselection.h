#ifndef VAULTSELECTION_H
#define VAULTSELECTION_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class VaultSelection;
}

class VaultSelection : public QWidget
{
    Q_OBJECT

public:
    explicit VaultSelection(QWidget *parent = nullptr);
    ~VaultSelection();

    void addVault(const QString &name);

signals:
    void createVaultRequested();
    void vaultSelected(const QString &vaultName); // emitted when a vault item is clicked

private:
    Ui::VaultSelection *ui;

    void updateArrowIcon(bool expanded);
};

#endif // VAULTSELECTION_H