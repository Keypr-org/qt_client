#ifndef NEWPERSONAFORM_H
#define NEWPERSONAFORM_H

#include <QWidget>

#include "model/persona.h"

namespace Ui {
class NewPersonaForm;
}

class NewPersonaForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewPersonaForm(QWidget *parent = nullptr);
    ~NewPersonaForm();

    void clearForm();

signals:
    void cancelSignal();
    void generatePersonaSignal();
    void usePersonaSignal(PersonaData persona);

private:
    Ui::NewPersonaForm *ui;
};

#endif // NEWPERSONAFORM_H
