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
    explicit VaultController(VaultRepository &repository);

    /**
     * @brief Check if a vault exists.
     * @param vaultName The name of the vault to check.
     * @return true if the vault exists, false otherwise.
     */
    bool vaultExists(const std::string &vaultName) const;

private:
    VaultRepository *repository;
    std::unique_ptr<VaultSession> session;
};
