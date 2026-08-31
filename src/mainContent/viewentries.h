#ifndef VIEWENTRIES_H
#define VIEWENTRIES_H

#include <QWidget>
#include <QtWidgets/qlistwidget.h>
#include <QStackedWidget>
#include <QString>
#include <memory>

#include "model/entry.h"
#include "model/entryrepository.h"
#include "model/personarepository.h"

class WebsiteEntry;
class WifiEntry;
class CreditCardEntry;

namespace Ui {
class ViewEntries;
}

class ViewEntries : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the entries list/detail view, seeding its own mock entry repository.
     * @param parent Parent widget, or nullptr.
     */
    explicit ViewEntries(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and its UI.
     */
    ~ViewEntries();

    /**
     * @brief Returns the repository backing the displayed entries.
     * @return Pointer to the entry repository owned by this widget.
     */
    EntryRepository *repository() const;

    /**
     * @brief Provides the persona source used by entry detail views to link/unlink personas.
     * @param repository Repository to read personas from.
     */
    void setPersonaRepository(PersonaRepository *repository);

    /**
     * @brief Re-populates the entries list from the repository's current state.
     */
    void refresh();

    /**
     * @brief Re-renders the currently selected entry's detail view from its (possibly just-changed) data, e.g. after a linked persona was deleted elsewhere.
     */
    void refreshCurrentEntryDetails();

    /**
     * @brief Clears the current list selection and shows the empty detail placeholder.
     */
    void clearSelection();

signals:
    /**
     * @brief Emitted when the user requests to create a new entry.
     */
    void newEntry();

private slots:
    /**
     * @brief Updates the visual selection state of list items when the current entry changes.
     * @param current Newly selected list item, or nullptr.
     * @param previous Previously selected list item, or nullptr.
     */
    void on_entriesList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::ViewEntries *ui;
    EntryRepository *m_repository;

    QStackedWidget *m_detailStack;
    QWidget *m_emptyDetailPage;
    WebsiteEntry *m_websiteEntryView;
    WifiEntry *m_wifiEntryView;
    CreditCardEntry *m_creditCardEntryView;

    QString m_searchFilter;

    /**
     * @brief Rebuilds the entries list widget from the repository, applying the current search filter.
     */
    void populateList();

    /**
     * @brief Checks whether an entry matches the current search filter.
     * @param entry Entry to test.
     * @return True if the entry should be shown, false otherwise.
     */
    bool matchesFilter(const std::shared_ptr<Entry> &entry) const;

    /**
     * @brief Shows the detail view corresponding to the given entry's type, populated with its data.
     * @param entry Entry to display.
     */
    void showEntryDetails(const std::shared_ptr<Entry> &entry);

    /**
     * @brief Removes an entry from the repository and refreshes the list/selection accordingly.
     * @param id Identifier of the entry to delete.
     */
    void handleDeleteRequested(const QString &id);

    /**
     * @brief Refreshes the list and re-selects the given entry after it has been updated.
     * @param id Identifier of the updated entry.
     */
    void handleEntryUpdated(const QString &id);
};

#endif // VIEWENTRIES_H
