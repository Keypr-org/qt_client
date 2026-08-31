#pragma once

#include "VaultSession.h"

#include <memory>
#include <vector>

class FakeVaultSession : public VaultSession {
public:
    FakeVaultSession() : VaultSession("", {}, {}) {};
};