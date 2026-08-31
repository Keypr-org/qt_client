#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include "fakevaultsession.h"
#include "VaultRepository.h"



/**
 * @brief A fake implementation of the VaultRepository interface for testing purposes.
 */
class FakeVaultRepository : public VaultRepository {
public:
    FakeVaultRepository(bool fileExists, bool parsingSuccess)
        : fileExists(fileExists), parsingSuccess(parsingSuccess) {
    };

    bool vaultExists(const std::string &vaultName) const override {
        if (fileExists && parsingSuccess) {
            return true;
        } else {
            return false;
        }
    }

private:
    bool fileExists;
    bool parsingSuccess;
};