#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include "VaultRepository.h"
#include "fakevaultsession.h"



/**
 * @brief A fake implementation of the VaultRepository interface for testing purposes.
 */
class FakeVaultRepository : public VaultRepository {
public:
    FakeVaultRepository(bool fileExistsOrGoodPassword, bool parsingSuccess)
        : fileExistsOrGoodPassword(fileExistsOrGoodPassword), parsingSuccess(parsingSuccess) {
    };

    bool vaultExists(const std::string &vaultName) const override {
        if (fileExistsOrGoodPassword && parsingSuccess) {
            return true;
        } else {
            return false;
        }
    }

    std::unique_ptr<VaultSession> unlockVault(const std::string &masterpass,
        const std::string &filename) const override {
        if (fileExistsOrGoodPassword && parsingSuccess) {
            return std::make_unique<FakeVaultSession>();
        } else if (!fileExistsOrGoodPassword) {
            return nullptr;
        } else {
            throw UnlockVaultError("Failed to unlock vault.");
        }
    }

    std::unique_ptr<VaultSession> createVault(const std::string &masterpass,
        const std::string &vaultName) const override {
        if (fileExistsOrGoodPassword || parsingSuccess) {
            return std::make_unique<FakeVaultSession>();
        } else {
            throw CreateVaultError("Failed to create vault.");
        }
    }

private:
    bool fileExistsOrGoodPassword;
    bool parsingSuccess;
};