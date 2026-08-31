#include "vaultcontroller.h"

#include <exception>

VaultController::VaultController(std::unique_ptr<VaultRepository> repository)
    : repository(std::move(repository)), session(nullptr) {
}

bool VaultController::vaultExists(const std::string &vaultName) const {
    if (repository == nullptr) {
        return false;
    }

    return repository->vaultExists(vaultName);
}
