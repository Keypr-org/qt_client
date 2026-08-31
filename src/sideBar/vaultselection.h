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
    /**
     * @brief Constructs the vault selection panel.
     * @param parent Parent widget.
     */
    explicit VaultSelection(QWidget *parent = nullptr);

    /**
     * @brief Destroys the panel and releases its UI resources.
     */
    ~VaultSelection();

    /**
     * @brief Appends a new vault entry to the list.
     * @param name Name of the vault to add.
     */
    void addVault(const QString &name);

signals:
    /**
     * @brief Emitted when the user requests creation of a new vault.
     */
    void createVaultRequested();

    /**
     * @brief Emitted when a vault item is clicked.
     * @param vaultName Name of the selected vault.
     */
    void vaultSelected(const QString &vaultName);

private:
    Ui::VaultSelection *ui;

    /**
     * @brief Swaps the expand/collapse arrow icon to reflect the panel's expanded state.
     * @param expanded Whether the panel is currently expanded.
     */
    void updateArrowIcon(bool expanded);
};

#endif // VAULTSELECTION_H