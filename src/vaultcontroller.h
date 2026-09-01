#pragma once

#include "VaultSession.h"
#include "VaultRepository.h"
#include <memory>

/**
 * @brief The VaultController class is responsible for managing the vaults.
 */
class VaultController
{
public:
    /**
     * @brief Construct a new VaultController object.
     * @param repository The vault repository to use.
     * @param pathToVaults The path to the directory containing the vaults.
     */
    explicit VaultController(std::unique_ptr<VaultRepository> repository, std::string pathToVaults = "");

    bool isVaultUnlocked() const;

    /**
     * @brief Check if a vault exists.
     * @param vaultName The name of the vault to check.
     * @return true if the vault exists, false otherwise.
     */
    bool vaultExists(const std::string &vaultName) const;

    /**
     * @brief Unlock a vault.
     * @param masterPassword The master password.
     * @param filename The name of the vault file to unlock.
     * @return true if the vault is unlocked, false otherwise.
     */
    bool unlockVault(const std::string &masterPassword, const std::string &filename);

    /**
     * @brief Create a new vault.
     * @param masterPassword The master password.
     * @param vaultName The name of the vault to create.
     * @return true if the vault is created, false otherwise.
     */
    bool createVault(const std::string &masterPassword, const std::string &vaultName);

    /**
     * @brief Lock the current vault.
     * @param filename The name of the vault file to lock.
     * @return true if the vault is locked, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool lockVault(const std::string &filename = "");

    /**
     * @brief Get the name of the current vault.
     * @return The name of the current vault.
     */
    const std::string &getVaultName() const;

    /**
     * @brief Get the categories in the current vault.
     * @return The categories in the current vault.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const std::vector<std::unique_ptr<Category>> &getCategories() const;

    /**
     * @brief Add a category to the current vault.
     * @param category The category to add.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    void addCategory(std::unique_ptr<Category> category);

    /**
     * @brief Get the personas in the current vault.
     * @return The personas in the current vault.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const std::vector<std::unique_ptr<Persona>> &getPersonas() const;

    /**
     * @brief Add a persona to the current vault.
     * @param persona The persona to add.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    void addPersona(std::unique_ptr<Persona> persona);

    /**
     * @brief Remove a persona.
     * @param personaId The ID of the persona to remove.
     * @return true if the persona is removed, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool removePersona(int64_t personaId);

    /**
     * @brief Link a persona to an entry in a category.
     * @param personaId The ID of the persona to link.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to link the persona to.
     * @return true if the persona is linked, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool linkPersonaToEntry(int64_t personaId, int64_t categoryId, int64_t entryId);

    /**
     * @brief Get a persona by its ID.
     * @param personaId The ID of the persona to get.
     * @return A reference to the persona with the specified ID.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const std::unique_ptr<Persona> &getPersonaById(int64_t personaId) const;

    /**
     * @brief Add an entry to a category.
     * @param categoryId The ID of the category to add the entry to.
     * @param entry The entry to add.
     * @return true if the entry is added, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool addEntryToCategory(int64_t categoryId, std::unique_ptr<Entry> entry);

    /**
     * @brief Remove an entry from a category.
     * @param categoryId The ID of the category.
     * @param entryId The ID of the entry to remove.
     * @return true if the entry is removed, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool removeEntryFromCategory(int64_t categoryId, int64_t entryId);

    /**
     * @brief Get all websites by their URL.
     * @param url The URL to search for.
     * @return A vector of pointers to the websites with the specified URL.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    std::vector<const Website *> getWebsitesByUrl(const std::string &url) const;

    /**
     * @brief Get a website by its ID.
     * @param entryId The ID of the website to get.
     * @return A pointer to the website with the specified ID, or nullptr if not found.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    const Website *getWebsiteById(int64_t entryId) const;

    /**
     * @brief Set the alias for a website.
     * @param categoryId The ID of the category containing the entry.
     * @param entryId The ID of the entry to set the alias for.
     * @param aliasId The ID of the alias to set.
     * @param alias The alias to set.
     * @return true if the alias is set, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool setAliasForWebsite(int64_t categoryId, int64_t entryId, const std::string &aliasId, const std::string &alias);

    /**
     * @brief Search for entries in a category by a search term.
     * @param categoryId The ID of the category to search in.
     * @param searchTerm The term to search for in the entries' notes, title, username, comments, ...
     * @return A vector of pointers to the entries that match the search term.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    std::vector<const Entry *> searchEntriesInCategory(int64_t categoryId, const std::string &searchTerm) const;

private:
    std::unique_ptr<VaultRepository> repository;
    std::unique_ptr<VaultSession> session;
    std::string pathToVaults;
};
