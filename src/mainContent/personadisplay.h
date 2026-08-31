#ifndef PERSONADISPLAY_H
#define PERSONADISPLAY_H

#include <QMap>
#include <QWidget>

#include "model/persona.h"

class PersonaItem;

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
    void updatePersona(const PersonaData &persona);
    void removePersona(const QString &id);

signals:
    void createPersona();
    void modifyPersonaRequested(PersonaData persona);
    void deletePersonaRequested(QString id);

private:
    Ui::PersonaDisplay *ui;

    static const int GRID_COLUMNS = 2;
    int m_personaCount = 0;
    QMap<QString, PersonaItem *> m_personaItems;
};

#endif // PERSONADISPLAY_H
