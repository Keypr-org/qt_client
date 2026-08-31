#ifndef WEBSITEENTRY_H
#define WEBSITEENTRY_H

#include <QWidget>
#include <memory>

#include "model/personarepository.h"
#include "model/websiteentrydata.h"

namespace Ui {
class WebsiteEntry;
}

class WebsiteEntry : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Builds the website entry detail widget.
     * @param parent Parent widget.
     */
    explicit WebsiteEntry(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and releases its UI resources.
     */
    ~WebsiteEntry();

    /**
     * @brief Populates the widget's fields from the given website entry, including its linked persona, if any.
     * @param entry Website entry data to display and edit.
     */
    void setEntry(const std::shared_ptr<WebsiteEntryData> &entry);

    /**
     * @brief Provides the persona source used to list, look up and display linkable personas.
     * @param repository Repository to read personas from.
     */
    void setPersonaRepository(PersonaRepository *repository);

signals:
    /**
     * @brief Emitted when the user requests deletion of the currently displayed entry.
     * @param id Identifier of the entry to delete.
     */
    void deleteRequested(QString id);

    /**
     * @brief Emitted after the currently displayed entry has been edited, linked to, or unlinked from a persona.
     * @param id Identifier of the updated entry.
     */
    void entryUpdated(QString id);

private:
    Ui::WebsiteEntry *ui;
    std::shared_ptr<WebsiteEntryData> m_entry;
    PersonaRepository *m_personaRepository = nullptr;

    /**
     * @brief Refreshes the linked-persona label and controls to reflect the current entry's persona state.
     */
    void refreshPersonaDisplay();

    /**
     * @brief Opens a picker listing available personas so the user can link one to the current entry.
     */
    void openPersonaPicker();

    /**
     * @brief Removes the persona link from the current entry, if one exists.
     */
    void unlinkPersona();

    /**
     * @brief Requests a new email alias from Postscale for the current entry and stores it.
     */
    void generateAlias();

    /**
     * @brief Clears the email alias stored on the current entry.
     */
    void deleteAlias();

    /**
     * @brief Shows or hides the alias success widget depending on whether the entry has an alias.
     */
    void refreshAliasDisplay();
};

#endif // WEBSITEENTRY_H
