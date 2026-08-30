#ifndef PERSONADISPLAY_H
#define PERSONADISPLAY_H

#include <QWidget>

#include "model/persona.h"

namespace Ui {
class PersonaDisplay;
}

class PersonaDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaDisplay(QWidget *parent = nullptr);
    ~PersonaDisplay();

    void addPersona(const PersonaData &persona);

signals:
    void createPersona();

private:
    Ui::PersonaDisplay *ui;

    static const int GRID_COLUMNS = 2;
    int m_personaCount = 0;
};

#endif // PERSONADISPLAY_H
