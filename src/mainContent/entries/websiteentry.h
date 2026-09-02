#ifndef WEBSITEENTRY_H
#define WEBSITEENTRY_H

#include <QWidget>

#include "../../utils/qtypes/QEntry.h"
#include "../../utils/qtypes/QPersona.h"

namespace Ui
{
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
     * @brief Populates the widget's fields from the given website entry.
     * @param entry Website entry data to display and edit.
     */
    void setEntry(const QEntry &entry);

    /**
     * @brief Provides the personas the user can link this entry to, and refreshes the linked
     * persona's displayed name.
     * @param personas Personas currently available in the vault.
     */
    void setAvailablePersonas(const QList<QPersona> &personas);

signals:
    /**
     * @brief Emitted when the user requests deletion of the currently displayed entry.
     * @param id Identifier of the entry to delete.
     */
    void deleteRequested(qint64 id);

    /**
     * @brief Emitted when the user applies edits to the currently displayed entry's fields.
     */
    void entrySaveRequested(qint64 id, QString username, QString password, QString url,
                            QString description, QString notes);

    /**
     * @brief Emitted when the user links a persona to the currently displayed entry.
     */
    void personaLinkRequested(qint64 id, qint64 personaId);

    /**
     * @brief Emitted when the user unlinks the persona from the currently displayed entry.
     */
    void personaUnlinkRequested(qint64 id);

    /**
     * @brief Emitted when a new mail alias was generated for the currently displayed entry.
     */
    void aliasSetRequested(qint64 id, QString aliasId, QString alias);

    /**
     * @brief Emitted when the user removes the mail alias from the currently displayed entry.
     */
    void aliasClearRequested(qint64 id);

private:
    Ui::WebsiteEntry *ui;
    qint64 m_entryId;
    qint64 m_personaId = -1;
    QString m_aliasId;
    QString m_alias;
    QList<QPersona> m_availablePersonas;

    /**
     * @brief Refreshes the linked-persona label to reflect the current entry's persona state.
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
