#ifndef ADDWEBSITEFORM_H
#define ADDWEBSITEFORM_H

#include <QWidget>

namespace Ui {
class AddWebsiteForm;
}

class AddWebsiteForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the website entry creation form.
     * @param parent Parent widget, or nullptr.
     */
    explicit AddWebsiteForm(QWidget *parent = nullptr);

    /**
     * @brief Destroys the form and its UI.
     */
    ~AddWebsiteForm();

    /**
     * @brief Pre-fills the password field, typically with a generated password.
     * @param password Password value to display in the field.
     */
    void setPassword(const QString &password);

    /**
     * @brief Resets all form fields to their empty/default state.
     */
    void clearForm();

signals:
    /**
     * @brief Emitted when the user requests to open the password generator.
     */
    void generatePassword();

    /**
     * @brief Emitted when the user submits the form with a valid new website entry.
     */
    void createWebsiteEntry(QString title, QString username, QString password, QString url,
                             QString description, QString notes);

    /**
     * @brief Emitted when the user cancels creating a new website entry.
     */
    void cancelNewWebEntry();

private:
    Ui::AddWebsiteForm *ui;
};

#endif // ADDWEBSITEFORM_H
