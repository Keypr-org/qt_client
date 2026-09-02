#ifndef VIEWENTRIES_H
#define VIEWENTRIES_H

#include <QWidget>
#include <QtWidgets/qlistwidget.h>
#include <QStackedWidget>
#include <QString>
#include <vector>

#include "../utils/qtypes/QEntry.h"

class WebsiteEntry;
class WifiEntry;
class CreditCardEntry;

namespace Ui
{
    class ViewEntries;
}

class ViewEntries : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the entries list/detail view.
     * @param parent Parent widget, or nullptr.
     */
    explicit ViewEntries(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and its UI.
     */
    ~ViewEntries();

    /**
     * @brief Loads and displays the entries of the given category from the vault.
     * @param categoryId Identifier of the category to display.
     */
    void loadCategory(qint64 categoryId);

    /**
     * @brief Clears the displayed entries and forgets the current category (e.g. when the
     * unlocked vault has no categories at all).
     */
    void clearCategory();

    /**
     * @brief Creates a new website entry in the current category.
     * @return true on success.
     */
    bool createWebsiteEntry(const QString &title, const QString &username, const QString &password,
                            const QString &url, const QString &description, const QString &notes);

    /**
     * @brief Creates a new wifi entry in the current category.
     * @return true on success.
     */
    bool createWifiEntry(const QString &networkName, const QString &password, const QString &notes);

    /**
     * @brief Creates a new credit card entry in the current category.
     * @return true on success.
     */
    bool createCreditCardEntry(const QString &cardHolderName, const QString &cardNumber,
                               const QString &expiration, const QString &securityCode,
                               const QString &notes);

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
    qint64 m_currentCategoryId = -1;
    std::vector<std::unique_ptr<QEntry>> m_entries;

    QStackedWidget *m_detailStack;
    QWidget *m_emptyDetailPage;
    WebsiteEntry *m_websiteEntryView;
    WifiEntry *m_wifiEntryView;
    CreditCardEntry *m_creditCardEntryView;

    QString m_searchFilter;

    /**
     * @brief Rebuilds the entries list widget from the currently loaded entries.
     */
    void populateList();

    /**
     * @brief Re-fetches m_entries for the current category, honoring the current search filter
     * (via VaultController::searchEntriesInCategory() when non-empty, or the full category
     * otherwise), and repopulates the list.
     */
    void refreshEntries();

    /**
     * @brief Finds a currently loaded entry by id.
     */
    const QEntry *findEntry(qint64 id) const;

    /**
     * @brief Shows the detail view corresponding to the given entry's kind, populated with its
     * data.
     */
    void showEntryDetails(const QEntry &entry);

    /**
     * @brief Re-selects the entry with the given id in the list, if present, and shows its
     * details.
     */
    void selectEntry(qint64 id);

    /**
     * @brief Reloads the current category and re-selects the given entry, e.g. after editing it.
     */
    void reloadAndReselect(qint64 id);

    /**
     * @brief Deletes an entry from the current category and refreshes the list/selection
     * accordingly.
     */
    void handleDeleteRequested(qint64 id);
};

#endif // VIEWENTRIES_H
