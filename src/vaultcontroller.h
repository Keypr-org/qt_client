#pragma once

#include "VaultSession.h"
#include "VaultRepository.h"
#include <memory>

/**
 * @brief The VaultController class is responsible for managing the vaults.
 */
class VaultController {
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
     * @param vaultName The name of the vault to unlock.
     * @return true if the vault is unlocked, false otherwise.
     */
    bool unlockVault(const std::string &masterPassword, const std::string &vaultName);

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

private:
    std::unique_ptr<VaultRepository> repository;
    std::unique_ptr<VaultSession> session;
};
