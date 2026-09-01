#include "vaultcontroller.h"

#include <exception>

VaultController::VaultController(std::unique_ptr<VaultRepository> repository)
    : repository(std::move(repository)), session(nullptr)
{
    if (this->repository == nullptr)
    {
        throw std::invalid_argument("Repository cannot be null");
    }
}

bool VaultController::isVaultUnlocked() const
{
    return session != nullptr;
}

bool VaultController::vaultExists(const std::string &vaultName) const
{
    return repository->vaultExists(vaultName);
}

bool VaultController::unlockVault(const std::string &masterPassword, const std::string &vaultName)
{
    try
    {
        session = repository->unlockVault(masterPassword, vaultName);
        return session != nullptr;
    }
    catch (UnlockVaultError &)
    {
        return false;
    }
}

bool VaultController::createVault(const std::string &masterPassword, const std::string &vaultName)
{
    try
    {
        session = repository->createVault(masterPassword, vaultName);
        return session != nullptr;
    }
    catch (CreateVaultError &)
    {
        return false;
    }
}

const std::vector<std::unique_ptr<Category>> &VaultController::getCategories() const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    return session->getCategories();
}

bool VaultController::removeEntryFromCategory(int64_t categoryId, int64_t entryId)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        session->removeEntryFromCategory(categoryId, entryId);
        return true;
    }
    catch (const CategoryNotFoundError &)
    {
        return false; // Category not found
    }
    catch (const EntryNotFoundError &)
    {
        return false; // Entry not found
    }
}

bool VaultController::linkPersonaToEntry(int64_t personaId, int64_t categoryId, int64_t entryId)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        session->linkPersonaToEntry(personaId, categoryId, entryId);
        return true;
    }
    catch (const PersonaNotFoundError &)
    {
        return false; // Persona not found
    }
    catch (const CategoryNotFoundError &)
    {
        return false; // Category not found
    }
    catch (const EntryNotFoundError &)
    {
        return false; // Entry not found
    }
    catch (const EntryNotGoodTypeError &)
    {
        return false; // Entry is not of the correct type to link a persona
    }
}

bool VaultController::removePersona(int64_t personaId)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        session->removePersona(personaId);
        return true;
    }
    catch (const PersonaNotFoundError &)
    {
        return false; // Persona not found
    }
}

bool VaultController::setAliasForWebsite(int64_t categoryId, int64_t entryId, const std::string &aliasId, const std::string &alias)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        session->setAliasForWebsite(categoryId, entryId, aliasId, alias);
        return true;
    }
    catch (const CategoryNotFoundError &)
    {
        return false; // Category not found
    }
    catch (const EntryNotFoundError &)
    {
        return false; // Entry not found
    }
    catch (const EntryNotGoodTypeError &)
    {
        return false; // Entry is not of the correct type to set an alias
    }
}