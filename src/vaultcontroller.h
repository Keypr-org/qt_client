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
     */
    explicit VaultController(std::unique_ptr<VaultRepository> repository);

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
     * @brief Get the categories in the current vault.
     * @return The categories in the current vault.
     */
    const std::vector<std::unique_ptr<Category>> &getCategories() const;

    /**
     * @brief Remove an entry from a category.
     * @param categoryId The ID of the category.
     * @param entryId The ID of the entry to remove.
     * @return true if the entry is removed, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool removeEntryFromCategory(int64_t categoryId, int64_t entryId);

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
     * @brief Remove a persona.
     * @param personaId The ID of the persona to remove.
     * @return true if the persona is removed, false otherwise.
     * @throws std::runtime_error if the vault session is not initialized (i.e., no vault is unlocked).
     */
    bool removePersona(int64_t personaId);

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

private:
    std::unique_ptr<VaultRepository> repository;
    std::unique_ptr<VaultSession> session;
};
