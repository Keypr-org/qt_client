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
    /**
     * @brief Constructs the persona grid display.
     * @param parent Parent widget, or nullptr.
     */
    explicit PersonaDisplay(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and its UI.
     */
    ~PersonaDisplay();

    /**
     * @brief Adds a new persona card to the grid.
     * @param persona Persona to display.
     */
    void addPersona(const PersonaData &persona);

    /**
     * @brief Refreshes the card matching the given persona's id with its new data.
     * @param persona Updated persona data.
     */
    void updatePersona(const PersonaData &persona);

    /**
     * @brief Removes a persona card from the grid and re-packs the remaining cards.
     * @param id Id of the persona to remove.
     */
    void removePersona(const QString &id);

signals:
    /**
     * @brief Emitted when the user requests to create a new persona.
     */
    void createPersona();

    /**
     * @brief Emitted when the user requests to edit an existing persona.
     * @param persona Persona to edit.
     */
    void modifyPersonaRequested(PersonaData persona);

    /**
     * @brief Emitted when the user requests to delete a persona.
     * @param id Id of the persona to delete.
     */
    void deletePersonaRequested(QString id);

private:
    Ui::PersonaDisplay *ui;

    static const int GRID_COLUMNS = 2;
    int m_personaCount = 0;
    QMap<QString, PersonaItem *> m_personaItems;
};

#endif // PERSONADISPLAY_H
