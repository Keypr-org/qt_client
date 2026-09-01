#ifndef VIEWENTRIES_H
#define VIEWENTRIES_H

#include <QWidget>
#include <QtWidgets/qlistwidget.h>
#include <QStackedWidget>
#include <QString>

#include "vaultbridge.h"

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
     * @brief Constructs the entries list/detail view.
     * @param parent Parent widget, or nullptr.
     */
    explicit ViewEntries(QWidget *parent = nullptr);

    /**
     * @brief Destroys the widget and its UI.
     */
    ~ViewEntries();

    /**
     * @brief Provides the bridge used to read and mutate entries. Must be called once before
     * loadCategory() or the create*Entry() methods are used.
     * @param bridge Bridge to the currently open vault. Not owned.
     */
    void setVaultBridge(VaultBridge *bridge);

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
    VaultBridge *m_vaultBridge = nullptr;
    qint64 m_currentCategoryId = -1;
    QList<VaultBridge::EntrySummary> m_entries;

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
    const VaultBridge::EntrySummary *findEntry(const QString &id) const;

    /**
     * @brief Shows the detail view corresponding to the given entry's kind, populated with its
     * data.
     */
    void showEntryDetails(const VaultBridge::EntrySummary &entry);

    /**
     * @brief Re-selects the entry with the given id in the list, if present, and shows its
     * details.
     */
    void selectEntry(const QString &id);

    /**
     * @brief Reloads the current category and re-selects the given entry, e.g. after editing it.
     */
    void reloadAndReselect(const QString &id);

    /**
     * @brief Deletes an entry from the current category and refreshes the list/selection
     * accordingly.
     */
    void handleDeleteRequested(const QString &id);
};

#endif // VIEWENTRIES_H
