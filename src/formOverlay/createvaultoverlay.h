#ifndef CREATEVAULTOVERLAY_H
#define CREATEVAULTOVERLAY_H

#include <QWidget>

namespace Ui {
class CreateVaultOverlay;
}

class CreateVaultOverlay : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the vault creation overlay.
     * @param parent Parent widget.
     */
    explicit CreateVaultOverlay(QWidget *parent = nullptr);

    /**
     * @brief Destroys the overlay and releases its UI resources.
     */
    ~CreateVaultOverlay();

    /**
     * @brief Resets the form fields to their empty/default state.
     */
    void clearForm();

private slots:
    /**
     * @brief Updates the strength label and rating lines to reflect the given password.
     * @param password Password to evaluate.
     */
    void updateStrengthDisplay(const QString &password);

signals:
    /**
     * @brief Emitted when the user cancels vault creation.
     */
    void cancelled();

    /**
     * @brief Emitted when the user submits valid new vault credentials.
     * @param name Name of the vault to create.
     * @param password Password to protect the new vault with.
     */
    void vaultCreated(const QString &name, const QString &password);

protected:
    /**
     * @brief Keeps the overlay's geometry in sync with its parent when resized.
     * @param event Resize event data.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::CreateVaultOverlay *ui;
};

#endif // CREATEVAULTOVERLAY_H