#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
class SettingsWindow;
}

class MailAliasController;

/**
 * @brief The SettingsWindow class lets the user configure app-wide settings,
 * currently the Postscale API key and source email used to create mail aliases.
 * Opened from the OS application menu (Preferences).
 */
class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow() override;

private:
    void loadCurrentSettings();

    Ui::SettingsWindow *ui;
    std::unique_ptr<MailAliasController> m_mailAliasController;
};
