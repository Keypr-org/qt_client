#ifndef PERSONADISPLAY_H
#define PERSONADISPLAY_H

#include <QWidget>

#include "../utils/qtypes/QPersona.h"

class PersonaItem;

namespace Ui
{
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
     * @brief Loads and displays every persona from the vault, replacing the current grid.
     */
    void loadPersonas();

    /**
     * @brief Creates a new persona in the vault and reloads the grid on success.
     * @return true on success.
     */
    bool addPersona(const QString &firstName, const QString &lastName, const QDate &dateOfBirth,
                    const QString &address, const QString &phone);

    /**
     * @brief Updates an existing persona's fields and reloads the grid on success.
     * @return true on success.
     */
    bool updatePersona(qint64 id, const QString &firstName, const QString &lastName,
                       const QDate &dateOfBirth, const QString &address, const QString &phone);

signals:
    /**
     * @brief Emitted when the user requests to create a new persona.
     */
    void createPersona();

    /**
     * @brief Emitted when the user requests to edit an existing persona.
     * @param persona Persona to edit.
     */
    void modifyPersonaRequested(const QPersona persona);

private:
    Ui::PersonaDisplay *ui;

    static const int GRID_COLUMNS = 2;
};

#endif // PERSONADISPLAY_H
