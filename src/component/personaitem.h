#ifndef PERSONAITEM_H
#define PERSONAITEM_H

#include <QWidget>

#include "vaultbridge.h"

namespace Ui {
class PersonaItem;
}

class PersonaItem : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a card widget displaying a single persona.
     * @param parent Parent widget.
     */
    explicit PersonaItem(QWidget *parent = nullptr);
    /**
     * @brief Destroys the widget and its UI.
     */
    ~PersonaItem();

    /**
     * @brief Sets the persona data displayed by this card.
     * @param persona The persona to display.
     */
    void setPersona(const VaultBridge::PersonaSummary &persona);
    /**
     * @brief Returns the persona currently displayed by this card.
     * @return The current persona data.
     */
    const VaultBridge::PersonaSummary &persona() const;

signals:
    /**
     * @brief Emitted when the user requests to modify this persona (e.g. via the edit button).
     * @param persona The persona to modify.
     */
    void modifyRequested(VaultBridge::PersonaSummary persona);
    /**
     * @brief Emitted when the user requests to delete this persona (e.g. via the delete button).
     * @param id ID of the persona to delete.
     */
    void deleteRequested(qint64 id);

protected:
    /**
     * @brief Repaints the card so its stylesheet applies to the custom QWidget background/border.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::PersonaItem *ui;
    VaultBridge::PersonaSummary m_persona{};
};

#endif // PERSONAITEM_H
