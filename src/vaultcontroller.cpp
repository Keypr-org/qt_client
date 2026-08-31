#include "vaultcontroller.h"

#include <exception>

VaultController::VaultController()
    : repository(nullptr) {
}

VaultController::VaultController(VaultRepository &repository)
    : repository(&repository) {
}

bool VaultController::vaultExists(const std::string &vaultName) const {
    if (repository == nullptr) {
        return false;
    }

    return repository->vaultExists(vaultName);
}
