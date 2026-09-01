#pragma once

#include "appconfig.h"

#include <QDialog>
#include <memory>

namespace Ui {
class SettingsWindow;
}

class MailAliasController;

/**
 * @brief The SettingsWindow class lets the user configure app-wide settings: the
 * Postscale API key and source email used to create mail aliases, and the folder
 * where vaults are stored. Opened from the OS application menu (Preferences).
 */
class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs a SettingsWindow object.
     * @param parent A pointer to the parent QWidget, default is nullptr.
     */
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow() override;

private:
    /**
     * @brief Loads the current settings from the AppConfig and updates the UI accordingly.
     */
    void loadCurrentSettings();
    
    /**
     * @brief Saves the current settings from the UI to the AppConfig.
     */
    void changeVaultPath();

    Ui::SettingsWindow *ui;
    std::unique_ptr<MailAliasController> m_mailAliasController;
    AppConfig m_appConfig;
    QString m_pendingVaultPath;
};
