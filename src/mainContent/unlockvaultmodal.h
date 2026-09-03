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

    /**
     * @brief Called by the owner once the unlock attempt succeeded, to complete the transition.
     */
    void reportUnlockSuccess();

    /**
     * @brief Called by the owner when the unlock attempt failed, to display the given error.
     * @param message Error message to display to the user.
     */
    void reportUnlockError(const QString &message);

signals:
    /**
     * @brief Emitted when the user submits the form to attempt unlocking the vault. The owner
     * is responsible for performing the actual check and calling reportUnlockSuccess() or
     * reportUnlockError() with the result.
     * @param vaultName Name of the vault to unlock.
     * @param password Master password entered by the user.
     */
    void unlockAttempted(const QString &vaultName, const QString &password);

    /**
     * @brief Emitted when the vault has been successfully unlocked.
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