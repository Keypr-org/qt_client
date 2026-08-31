#ifndef EDITPERSONAOVERLAY_H
#define EDITPERSONAOVERLAY_H

#include <QWidget>

#include "model/persona.h"

namespace Ui {
class EditPersonaOverlay;
}

class EditPersonaOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit EditPersonaOverlay(QWidget *parent = nullptr);
    ~EditPersonaOverlay();

    void setPersona(const PersonaData &persona);

signals:
    void cancelled();
    void personaModified(PersonaData persona);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::EditPersonaOverlay *ui;
    QString m_personaId;
};

#endif // EDITPERSONAOVERLAY_H
