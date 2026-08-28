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

    explicit PasswordInput(QWidget *parent = nullptr);
    ~PasswordInput();

    void setLabelText(const QString &text);

    // Adds an action and returns its ID to remove it
    QString addAction(const QIcon &icon, const QString &tooltip, ActionCallback onClick);

    // Removes an action by its ID
    void removeAction(const QString &id);

    // Toggles the eye
    void setEyeIconEnabled(bool enabled);

    // Adds the copy action
    void addCopyToClipboardAction();

    void setLabelVisible(bool visible);

    void setText(const QString &text);

    QString text() const;

private:
    Ui::PasswordInput *ui;
};

#endif // PASSWORDINPUT_H