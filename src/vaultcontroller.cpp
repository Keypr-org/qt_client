#include "vaultcontroller.h"

#include <exception>

VaultController::VaultController(std::unique_ptr<VaultRepository> repository)
    : repository(std::move(repository)), session(nullptr) {
    if (this->repository == nullptr) {
        throw std::invalid_argument("Repository cannot be null");
    }
}

bool VaultController::isVaultUnlocked() const {
    return session != nullptr;
}

bool VaultController::vaultExists(const std::string &vaultName) const {
    return repository->vaultExists(vaultName);
}

bool VaultController::unlockVault(const std::string &masterPassword, const std::string &vaultName) {
    try {
        session = repository->unlockVault(masterPassword, vaultName);
        return session != nullptr;
    }
    catch (UnlockVaultError &) {
        return false;
    }
}
const std::vector<std::unique_ptr<Category>> &VaultController::getCategories() const {
    if (session == nullptr) {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    return session->getCategories();
}