#ifndef UNLOCKVAULTMODAL_H
#define UNLOCKVAULTMODAL_H

#include <QWidget>

namespace Ui {
class unlockvaultmodal;
}

class unlockvaultmodal : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the vault unlock modal.
     * @param parent Parent widget, or nullptr.
     */
    explicit unlockvaultmodal(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and its UI.
     */
    ~unlockvaultmodal();

    /**
     * @brief Sets the vault name displayed to the user in the modal.
     * @param name Name of the vault being unlocked.
     */
    void setVaultName(const QString &name);

signals:
    /**
     * @brief Emitted when the user successfully unlocks the vault.
     * @param name Name of the vault that was unlocked.
     */
    void vaultUnlocked(const QString &name);

private slots:
    /**
     * @brief Attempts to unlock the vault with the entered password when the unlock button is clicked.
     */
    void onUnlockClicked();

private:
    Ui::unlockvaultmodal *ui;
    QString m_vaultName;
};

#endif // UNLOCKVAULTMODAL_H