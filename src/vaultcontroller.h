#pragma once

#include "VaultSession.h"
#include "VaultRepository.h"
#include <memory>
#include <QString>
#include <QList>
#include <vector>
#include "utils/qtypes/QEntry.h"
#include "utils/qtypes/QCategory.h"
#include "utils/qtypes/QPersona.h"
#include "utils/qtypes/QWebsite.h"
#include "utils/qtypes/QCreditCard.h"
#include "utils/qtypes/QWifi.h"

/**
 * @brief The VaultController class is responsible for managing the vaults. implements pattern
 */
class VaultController
{
public:
    /**
     * @brief The result of an unlock operation.
     */
    enum class UnlockResult
    {
        Success,
        IncorrectPasswordOrCorrupted,
    };

    /**
     * @brief Get the singleton instance of the VaultController.
     * @param pathToVaults The path to the directory containing the vaults.
     * @param repository The vault repository to use.
     * @return A reference to the singleton instance.
     */
    static VaultController &getInstance(const QString &pathToVaults = "", std::unique_ptr<VaultRepository> repository = nullptr);

    void setVaultStoragePath(const QString &path)
    {
        pathToVaults = path.toStdString();
    }

    bool isVaultUnlocked() const;

    /**
     * @brief Check if a vault exists.
     * @param vaultName The name of the vault to check.
     * @return true if the vault exists, false otherwise.
     */
    bool vaultExists(const QString &vaultName) const;

    /**
     * @brief Unlock a vault.
     * @param masterPassword The master password.
     * @param filename The name of the vault file to unlock.
     * @return The result of the unlock operation.
     */
    UnlockResult unlockVault(const QString &masterPassword, const QString &filename);

    /**
     * @brief Create a new vault.
     * @param masterPassword The master password.
     * @param vaultName The name of the vault to create.
     * @return true if the vault is created, false otherwise.
     */
    bool createVault(const QString &masterPassword, const QString &vaultName);

    /**
     * @brief Lock the current vault.
     * @param filename The name of the vault file to lock.
     * @return true if the vault is locked, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     * @throws std::runtime_error if no filename is provided and the path to vaults is not set.
     */
    bool lockVault(const QString &filename = "");

    /**
     * @brief Get the name of the current vault.
     * @return The name of the current vault.
     */
    const QString getVaultName() const;

    /**
     * @brief Get the categories in the current vault.
     * @return The categories in the current vault.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const QList<QCategory> getCategories() const;

    /**
     * @brief Add a category to the current vault.
     * @param name The name of the category to add.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    void addCategory(const QString &name);

    /**
     * @brief Get the entries in a category.
     * @param categoryId The ID of the category.
     * @return A list of pointers to the entries in the category or an empty list if the category is not found.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    std::vector<std::unique_ptr<QEntry>> getEntriesInCategory(qint64 categoryId) const;

    /**
     * @brief Get the personas in the current vault.
     * @return The personas in the current vault.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const QList<QPersona> getPersonas() const;

    /**
     * @brief Add a persona to the current vault.
     * @param firstName The first name of the persona.
     * @param lastName The last name of the persona.
     * @param dateOfBirth The date of birth of the persona.
     * @param address The address of the persona.
     * @param phone The phone number of the persona.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    void addPersona(const QString &firstName, const QString &lastName, const QDate &dateOfBirth,
                    const QString &address, const QString &phone);

    /**
     * @brief Remove a persona.
     * @param personaId The ID of the persona to remove.
     * @return true if the persona is removed, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool removePersona(qint64 personaId);

    /**
     * @brief Update a persona.
     * @param personaId The ID of the persona to update.
     * @param firstName The new first name of the persona.
     * @param lastName The new last name of the persona.
     * @param dateOfBirth The new date of birth of the persona.
     * @param address The new address of the persona.
     * @param phone The new phone number of the persona.
     * @return true if the persona is updated, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool updatePersona(qint64 personaId, const QString &firstName, const QString &lastName,
                       const QDate &dateOfBirth, const QString &address, const QString &phone);

    /**
     * @brief Link a persona to an entry in a category.
     * @param personaId The ID of the persona to link.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to link the persona to.
     * @return true if the persona is linked, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool linkPersonaToEntry(qint64 personaId, qint64 categoryId, qint64 entryId);

    /**
     * @brief Unlink the persona currently linked to an entry in a category, if any.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to unlink the persona from.
     * @return true if the persona is unlinked, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool unlinkPersonaFromEntry(qint64 categoryId, qint64 entryId);

    /**
     * @brief Get a persona by its ID.
     * @param personaId The ID of the persona to get.
     * @return A reference to the persona with the specified ID.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const QPersona getPersonaById(qint64 personaId) const;

    /**
     * @brief Creates a new website entry in the given category
     * @param categoryId The ID of the category to add the entry to.
     * @param title The title of the website entry.
     * @param username The username for the website entry.
     * @param password The password for the website entry.
     * @param url The URL for the website entry.
     * @param comments Any comments for the website entry.
     * @param notes Any notes for the website entry.
     * @return true if the entry is added, false otherwise.
     */
    bool addWebsiteEntry(qint64 categoryId, const QString &title, const QString &username,
                         const QString &password, const QString &url, const QString &comments,
                         const QString &notes);

    /**
     * @brief Creates a new wifi entry in the given category.
     * @param categoryId The ID of the category to add the entry to.
     * @param networkName The network name of the wifi entry.
     * @param password The password for the wifi entry.
     * @param notes Any notes for the wifi entry.
     * @return true if the entry is added, false otherwise.
     */
    bool addWifiEntry(qint64 categoryId, const QString &networkName, const QString &password,
                      const QString &notes);

    /**
     * @brief Creates a new credit card entry in the given category.
     * @param categoryId The ID of the category to add the entry to.
     * @param cardHolderName The cardholder name of the credit card entry.
     * @param cardNumber The card number for the credit card entry.
     * @param expiration The expiration date for the credit card entry.
     * @param securityCode The security code for the credit card entry.
     * @param notes Any notes for the credit card entry.
     * @return true if the entry is added, false otherwise.
     */
    bool addCreditCardEntry(qint64 categoryId, const QString &cardHolderName,
                            const QString &cardNumber, const QString &expiration,
                            const QString &securityCode, const QString &notes);

    /**
     * @brief Update a website entry.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to update.
     * @param username The new username for the website entry.
     * @param password The new password for the website entry.
     * @param url The new URL for the website entry.
     * @param comments The new comments for the website entry.
     * @param notes The new notes for the website entry.
     * @return true if the entry is updated, false otherwise.
     */
    bool updateWebsiteEntry(const qint64 categoryId, const qint64 entryId, const QString &username, const QString &password,
                            const QString &url, const QString &comments, const QString &notes);

    /**
     * @brief Update a wifi entry.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to update.
     * @param networkName The new network name of the wifi entry.
     * @param password The new password for the wifi entry.
     * @param notes The new notes for the wifi entry.
     * @return true if the entry is updated, false otherwise.
     */
    bool updateWifiEntry(const qint64 categoryId, const qint64 entryId, const QString &networkName, const QString &password,
                         const QString &notes);

    /**
     * @brief Update a credit card entry.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to update.
     * @param cardHolderName The new cardholder name of the credit card entry.
     * @param cardNumber The new card number for the credit card entry.
     * @param expiration The new expiration date for the credit card entry.
     * @param securityCode The new security code for the credit card entry.
     * @param notes The new notes for the credit card entry.
     * @return true if the entry is updated, false otherwise.
     */
    bool updateCreditCardEntry(const qint64 categoryId, const qint64 entryId, const QString &cardHolderName,
                               const QString &cardNumber, const QString &expiration,
                               const QString &securityCode, const QString &notes);

    /**
     * @brief Remove an entry from a category.
     * @param categoryId The ID of the category.
     * @param entryId The ID of the entry to remove.
     * @return true if the entry is removed, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool removeEntryFromCategory(qint64 categoryId, qint64 entryId);

    /**
     * @brief Get all websites by their URL.
     * @param url The URL to search for.
     * @return A list of pointers to the websites with the specified URL.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    std::vector<QWebsite> getWebsitesByUrl(const QString &url) const;

    /**
     * @brief Get a website by its ID.
     * @param entryId The ID of the website to get.
     * @return A pointer to the website with the specified ID, or nullptr if not found.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const QWebsite getWebsiteById(qint64 entryId) const;

    /**
     * @brief Set the alias for a website.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to set the alias for.
     * @param aliasId The ID of the alias to set.
     * @param alias The alias to set.
     * @return true if the alias is set, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool setAliasForWebsite(qint64 categoryId, qint64 entryId, const QString &aliasId, const QString &alias);

    /**
     * @brief Search for entries in a category by a search term.
     * @param categoryId The ID of the category to search in.
     * @param searchTerm The term to search for in the entries' notes, title, username, comments, ...
     * @return A vector of pointers to the entries that match the search term.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    std::vector<std::unique_ptr<QEntry>> searchEntriesInCategory(qint64 categoryId, const QString &searchTerm) const;

    /**
     * @brief The path a vault with the given name would be written to / read from.
     */
    QString vaultFilePath(const QString &vaultName) const;

private:
    /**
     * @brief Construct a new VaultController object.
     * @param repository The vault repository to use.
     * @param pathToVaults The path to the directory containing the vaults.
     */
    explicit VaultController(const QString &pathToVaults = "", std::unique_ptr<VaultRepository> repository = nullptr);

    /**
     * @brief Sanitizes a vault name to create a valid filename for the vault file.
     * @param vaultName The original vault name.
     * @return A sanitized string suitable for use as a filename.
     */
    QString sanitizedVaultFileStem(const QString &vaultName) const;

    /**
     * @brief Add an entry to a category.
     * @param categoryId The ID of the category to add the entry to.
     * @param entry The entry to add.
     * @return true if the entry is added, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool addEntryToCategory(int64_t categoryId, std::unique_ptr<Entry> entry);

    /**
     * @brief Get an entry by its ID from a category.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to get.
     * @return A reference to the entry with the specified ID, or a reference to a nullptr if not found.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const std::unique_ptr<Entry> &getEntryByIdFromCategory(int64_t categoryId, int64_t entryId) const;

    std::unique_ptr<VaultRepository> repository;
    std::unique_ptr<VaultSession> session;
    std::string pathToVaults;

    static std::unique_ptr<VaultController> instance;

    friend class VaultControllerTest;
};
