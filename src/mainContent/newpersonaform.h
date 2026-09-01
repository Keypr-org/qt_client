#ifndef NEWPERSONAFORM_H
#define NEWPERSONAFORM_H

#include <QWidget>
#include <QDate>

namespace Ui {
class NewPersonaForm;
}

class NewPersonaForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the new persona creation form.
     * @param parent Parent widget, or nullptr.
     */
    explicit NewPersonaForm(QWidget *parent = nullptr);

    /**
     * @brief Destroys the form and its UI.
     */
    ~NewPersonaForm();

    /**
     * @brief Resets all form fields to their empty/default state.
     */
    void clearForm();

signals:
    /**
     * @brief Emitted when the user cancels creating a new persona.
     */
    void cancelSignal();

    /**
     * @brief Emitted when the user requests a randomly generated persona.
     */
    void generatePersonaSignal();

    /**
     * @brief Emitted when the user submits the form with a valid new persona.
     */
    void usePersonaSignal(QString firstName, QString lastName, QDate dateOfBirth, QString address,
                           QString phone);

private:
    Ui::NewPersonaForm *ui;
};

#endif // NEWPERSONAFORM_H
