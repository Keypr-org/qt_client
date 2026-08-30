#ifndef NEWPERSONAFORM_H
#define NEWPERSONAFORM_H

#include <QWidget>

namespace Ui {
class NewPersonaForm;
}

class NewPersonaForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewPersonaForm(QWidget *parent = nullptr);
    ~NewPersonaForm();

signals:
    void cancelSignal();
    void generatePersonaSignal();
    void usePersonaSignal();

private:
    Ui::NewPersonaForm *ui;
};

#endif // NEWPERSONAFORM_H
