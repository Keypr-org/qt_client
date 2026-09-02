#ifndef EDITPERSONAOVERLAY_H
#define EDITPERSONAOVERLAY_H

#include <QWidget>
#include <QDate>

#include "../utils/qtypes/QPersona.h"

namespace Ui
{
    class EditPersonaOverlay;
}

class EditPersonaOverlay : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the persona edition overlay.
     * @param parent Parent widget.
     */
    explicit EditPersonaOverlay(QWidget *parent = nullptr);

    /**
     * @brief Destroys the overlay and releases its UI resources.
     */
    ~EditPersonaOverlay();

    /**
     * @brief Populates the form fields with an existing persona's data for editing.
     * @param persona Persona to load into the form.
     */
    void setPersona(const QPersona &persona);

signals:
    /**
     * @brief Emitted when the user cancels the edition.
     */
    void cancelled();

    /**
     * @brief Emitted when the user submits the modified persona.
     */
    void personaModified(qint64 id, QString firstName, QString lastName, QDate dateOfBirth,
                         QString address, QString phone);

protected:
    /**
     * @brief Keeps the overlay's geometry in sync with its parent when resized.
     * @param event Resize event data.
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::EditPersonaOverlay *ui;
    qint64 m_personaId = -1;
};

#endif // EDITPERSONAOVERLAY_H
