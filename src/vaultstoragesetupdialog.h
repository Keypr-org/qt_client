#pragma once

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

/**
 * @brief Mandatory first-run dialog that makes the user choose the directory where
 * vaults are stored, persisting it to AppConfig. Cannot be dismissed without
 * choosing a valid directory: the close button, Escape and reject() are all blocked.
 */
class VaultStorageSetupDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs a VaultStorageSetupDialog object.
     * @param parent A pointer to the parent QWidget, default is nullptr.
     */
    explicit VaultStorageSetupDialog(QWidget *parent = nullptr);

protected:
    /**
     * @brief Overrides the close event to prevent closing the dialog without a valid selection.
     * @param event The close event.
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief Overrides the key press event to block the Escape key from closing the dialog.
     * @param event The key press event.
     */
    void keyPressEvent(QKeyEvent *event) override;
    
    /**
     * @brief Overrides the reject method to prevent closing the dialog without a valid selection.
     */
    void reject() override;

private:
    /**
     * @brief Opens a directory selection dialog and updates the selected path.
     */
    void browseForDirectory();
    
    /**
     * @brief Validates the selected path and saves it to AppConfig if valid.
     */
    void confirm();

    QLabel *m_pathLabel;
    QLabel *m_errorLabel;
    QPushButton *m_browseButton;
    QPushButton *m_confirmButton;
    QString m_selectedPath;
};
