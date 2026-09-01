#pragma once

#include <QDate>
#include <QDateTime>
#include <QList>
#include <QString>
#include <QStringList>
#include <memory>

class VaultController;

/**
 * @brief Thin bridge to VaultController for vault creation and unlocking. Keeps keypr-core's
 * entity headers (pulled in by vaultcontroller.h) out of translation units like mainwindow.cpp,
 * which still include the legacy src/model/entry.h types that define a clashing, unrelated
 * "Entry" class.
 */
class VaultBridge {
public:
    enum class UnlockResult {
        Success,
        VaultFileNotFound,
        IncorrectPasswordOrCorrupted,
    };

    explicit VaultBridge(const QString &vaultStoragePath);
    ~VaultBridge();

    /**
     * @brief The path a vault with the given name would be written to / read from.
     */
    QString vaultFilePath(const QString &vaultName) const;

    /**
     * @brief Creates a new vault and persists it to disk (see VaultController::createVault()
     * and VaultController::lockVault()).
     * @return true on success.
     */
    bool createVault(const QString &vaultName, const QString &masterPassword);

    /**
     * @brief Attempts to unlock the vault with the given name using the given master password
     * (see VaultController::unlockVault()).
     */
    UnlockResult unlockVault(const QString &vaultName, const QString &masterPassword);

    /**
     * @brief Persists the currently unlocked vault to disk and locks it (see
     * VaultController::lockVault()).
     * @return true on success, false if no vault is currently unlocked or saving failed.
     */
    bool lockVault();

    /**
     * @brief A category's identifier and display name.
     */
    struct CategorySummary {
        qint64 id;
        QString name;
    };

    enum class EntryKind {
        Website,
        Wifi,
        CreditCard,
    };

    /**
     * @brief A flattened, framework-agnostic view of a single vault entry, covering the fields
     * of every supported entry kind (see EntryKind). Only the fields relevant to `kind` are set.
     */
    struct EntrySummary {
        QString id;
        EntryKind kind = EntryKind::Website;
        QString notes;
        QDateTime lastUpdated;

        // Website
        QString title;
        QString username;
        QString password;
        QString url;
        QString comments;
        QString aliasId;
        QString alias;

        // Wifi (reuses password above)
        QString networkName;

        // CreditCard
        QString cardHolderName;
        QString cardNumber;
        QString expiration;
        QString securityCode;

        // Website only: id of the linked persona, or -1 if none.
        qint64 personaId = -1;
    };

    /**
     * @brief A persona's identifier and fields (see keypr-core's Persona entity). There is no
     * gender/country here: the real Persona entity doesn't have those fields.
     */
    struct PersonaSummary {
        qint64 id;
        QString firstName;
        QString lastName;
        QDate dateOfBirth;
        QString address;
        QString phone;
    };

    /**
     * @brief The categories in the currently unlocked vault (see
     * VaultController::getCategories()).
     * @return The categories, or an empty list if no vault is currently unlocked.
     */
    QList<CategorySummary> categories() const;

    /**
     * @brief Adds a new category to the currently unlocked vault (see
     * VaultController::addCategory()).
     * @return true on success, false if no vault is currently unlocked.
     */
    bool addCategory(const QString &name);

    /**
     * @brief The entries in the given category of the currently unlocked vault (see
     * VaultController::getEntriesInCategory()).
     * @return The entries, or an empty list if no vault is currently unlocked or the category
     * does not exist.
     */
    QList<EntrySummary> entriesInCategory(qint64 categoryId) const;

    /**
     * @brief The entries in the given category whose notes, or type-specific fields (title,
     * username, url, comments and alias for websites; network name for wifi; cardholder name for
     * credit cards), contain the given search term (see VaultController::searchEntriesInCategory()
     * — matching is a plain, case-sensitive substring search). An empty search term matches
     * every entry.
     * @return The matching entries, or an empty list if no vault is currently unlocked or the
     * category does not exist.
     */
    QList<EntrySummary> searchEntriesInCategory(qint64 categoryId, const QString &searchTerm) const;

    /**
     * @brief Creates a new website entry in the given category (see
     * VaultController::addEntryToCategory()).
     * @return The new entry's id, or an empty string on failure.
     */
    QString addWebsiteEntry(qint64 categoryId, const QString &title, const QString &username,
                             const QString &password, const QString &url, const QString &comments,
                             const QString &notes);

    /**
     * @brief Creates a new wifi entry in the given category.
     * @return The new entry's id, or an empty string on failure.
     */
    QString addWifiEntry(qint64 categoryId, const QString &networkName, const QString &password,
                          const QString &notes);

    /**
     * @brief Creates a new credit card entry in the given category.
     * @return The new entry's id, or an empty string on failure.
     */
    QString addCreditCardEntry(qint64 categoryId, const QString &cardHolderName,
                                const QString &cardNumber, const QString &expiration,
                                const QString &securityCode, const QString &notes);

    /**
     * @brief Removes an entry from the given category (see
     * VaultController::removeEntryFromCategory()).
     */
    bool removeEntry(qint64 categoryId, const QString &entryId);

    /**
     * @brief Updates the fields of an existing website entry in place. VaultController exposes
     * no generic "update entry" primitive, so this mutates the real Website object directly
     * (found via VaultController::getCategories()/Category::findEntryById(), both of which hand
     * back a mutable pointer even though the outer containers are const-qualified).
     */
    bool updateWebsiteEntry(const QString &entryId, const QString &username, const QString &password,
                             const QString &url, const QString &comments, const QString &notes);

    /**
     * @brief Updates the fields of an existing wifi entry in place (see updateWebsiteEntry()).
     */
    bool updateWifiEntry(const QString &entryId, const QString &networkName, const QString &password,
                          const QString &notes);

    /**
     * @brief Updates the fields of an existing credit card entry in place (see
     * updateWebsiteEntry()).
     */
    bool updateCreditCardEntry(const QString &entryId, const QString &cardHolderName,
                                const QString &cardNumber, const QString &expiration,
                                const QString &securityCode, const QString &notes);

    /**
     * @brief Links a persona to a website entry (see VaultController::linkPersonaToEntry()),
     * which validates the persona, category and entry all exist and that the entry is a website
     * before linking.
     * @return true on success, false if the persona/category/entry doesn't exist or the entry
     * isn't a website.
     */
    bool linkPersonaToWebsite(qint64 categoryId, const QString &entryId, qint64 personaId);

    /**
     * @brief Removes the persona link from a website entry in place, if any.
     */
    bool unlinkPersonaFromWebsite(const QString &entryId);

    /**
     * @brief Sets or clears (pass empty strings) the mail alias on a website entry in place.
     */
    bool setWebsiteAlias(const QString &entryId, const QString &aliasId, const QString &alias);

    /**
     * @brief The personas in the currently unlocked vault (see VaultController::getPersonas()).
     * @return The personas, or an empty list if no vault is currently unlocked.
     */
    QList<PersonaSummary> personas() const;

    /**
     * @brief Creates a new persona in the currently unlocked vault (see
     * VaultController::addPersona()).
     * @return The new persona's id, or -1 on failure.
     */
    qint64 addPersona(const QString &firstName, const QString &lastName, const QDate &dateOfBirth,
                       const QString &address, const QString &phone);

    /**
     * @brief Updates the fields of an existing persona (see VaultController::getPersonaById(),
     * whose returned reference allows in-place mutation via Persona's own setters).
     */
    bool updatePersona(qint64 personaId, const QString &firstName, const QString &lastName,
                        const QDate &dateOfBirth, const QString &address, const QString &phone);

    /**
     * @brief Removes a persona from the currently unlocked vault (see
     * VaultController::removePersona()).
     */
    bool removePersona(qint64 personaId);

private:
    QString m_vaultStoragePath;
    std::unique_ptr<VaultController> m_controller;
};
