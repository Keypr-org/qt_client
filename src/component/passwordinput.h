#ifndef PASSWORDINPUT_H
#define PASSWORDINPUT_H

#include <QWidget>
#include <QIcon>
#include <QString>
#include <functional>

namespace Ui {
class PasswordInput;
}

class QLineEdit;

class PasswordInput : public QWidget
{
    Q_OBJECT

public:
    using ActionCallback = std::function<void(QLineEdit*)>;

    /**
     * @brief Constructs a labeled password input with a masking toggle and optional actions.
     * @param parent Parent widget.
     */
    explicit PasswordInput(QWidget *parent = nullptr);
    /**
     * @brief Destroys the widget and its UI.
     */
    ~PasswordInput();

    /**
     * @brief Sets the field's label text.
     * @param text The label to display.
     */
    void setLabelText(const QString &text);

    /**
     * @brief Adds a clickable action (icon button) to the field.
     * @param icon Icon shown for the action.
     * @param tooltip Tooltip text shown on hover.
     * @param onClick Callback invoked when the action is clicked.
     * @return The action's ID, usable to remove it later via removeAction().
     */
    QString addAction(const QIcon &icon, const QString &tooltip, ActionCallback onClick);

    /**
     * @brief Removes a previously added action.
     * @param id The ID returned by addAction().
     */
    void removeAction(const QString &id);

    /**
     * @brief Enables or disables the show/hide (eye) toggle for the masked password.
     * @param enabled True to show the eye icon toggle.
     */
    void setEyeIconEnabled(bool enabled);

    /**
     * @brief Adds a built-in "copy to clipboard" action to the field.
     */
    void addCopyToClipboardAction();

    /**
     * @brief Shows or hides the field's label.
     * @param visible True to show the label.
     */
    void setLabelVisible(bool visible);

    /**
     * @brief Sets the field's current text content.
     * @param text The text to set.
     */
    void setText(const QString &text);

    /**
     * @brief Returns the field's current text content.
     * @return The current text.
     */
    QString text() const;

private:
    Ui::PasswordInput *ui;
};

#endif // PASSWORDINPUT_H