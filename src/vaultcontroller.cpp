#include "vaultcontroller.h"

#include <exception>
#include <QFileInfo>
#include <QDateTime>
#include <QTime>
#include <QDir>

#include "entities/Website.h"
#include "entities/Wifi.h"
#include "entities/CreditCard.h"

std::unique_ptr<VaultController> VaultController::instance = nullptr;

VaultController &VaultController::getInstance(const QString &pathToVaults, std::unique_ptr<VaultRepository> repository)
{
    if (instance == nullptr)
    {
        instance = std::unique_ptr<VaultController>(new VaultController(pathToVaults, std::move(repository)));
    }
    return *instance;
}

VaultController::VaultController(const QString &pathToVaults, std::unique_ptr<VaultRepository> repository)
    : repository(std::move(repository)), session(nullptr), pathToVaults(pathToVaults.toStdString())
{
    if (this->repository == nullptr)
    {
        this->repository = std::make_unique<VaultRepository>();
    }
}

bool VaultController::isVaultUnlocked() const
{
    return session != nullptr;
}

bool VaultController::vaultExists(const QString &vaultName) const
{
    return repository->vaultExists(vaultName.toStdString());
}

VaultController::UnlockResult VaultController::unlockVault(const QString &masterPassword, const QString &vaultName)
{
    const QString path = vaultFilePath(vaultName);
    try
    {
        session = repository->unlockVault(masterPassword.toStdString(), path.toStdString());
        if (session == nullptr)
        {
            return UnlockResult::IncorrectPasswordOrCorrupted;
        }
        return UnlockResult::Success;
    }
    catch (UnlockVaultError &)
    {
        return UnlockResult::IncorrectPasswordOrCorrupted;
        ;
    }
}

bool VaultController::createVault(const QString &masterPassword, const QString &vaultName)
{
    try
    {
        session = repository->createVault(masterPassword.toStdString(), vaultName.toStdString());
        return lockVault();
    }
    catch (CreateVaultError &)
    {
        return false;
    }
}

bool VaultController::lockVault(const QString &filename)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    std::string path;
    if (filename.isEmpty())
    {
        if (pathToVaults.empty())
        {
            throw std::runtime_error("Path to vaults is not set. Please provide a path to the vaults.");
        }
        std::string vaultName = session->getName();
        std::transform(vaultName.begin(), vaultName.end(), vaultName.begin(),
                       [](unsigned char c)
                       {
                           if (std::isalnum(c))
                           {
                               return static_cast<char>(std::tolower(c));
                           }
                           else
                           {
                               return '-';
                           }
                       });
        path = pathToVaults + vaultName + ".kvdb";
    }
    else
    {
        path = filename.toStdString();
    }

    if (repository->lockVault(*session, path))
    {
        session.reset(); // Reset the session to indicate that the vault is locked
        return true;
    }
    else
    {
        return false;
    }
}

const QString VaultController::getVaultName() const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    return QString::fromStdString(session->getName());
}

const QList<QCategory> VaultController::getCategories() const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }

    QList<QCategory> categories;
    for (const auto &cat : session->getCategories())
    {
        categories.append(QCategory(cat));
    }
    return categories;
}

void VaultController::addCategory(const QString &name)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    session->addCategory(std::make_unique<Category>(name.toStdString()));
}

std::vector<std::unique_ptr<QEntry>> VaultController::getEntriesInCategory(qint64 categoryId) const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        std::vector<std::unique_ptr<QEntry>> entries;
        for (const auto &entry : session->getEntriesInCategory(categoryId))
        {
            std::unique_ptr<QEntry> qEntry;
            if (auto websiteEntry = dynamic_cast<Website *>(entry.get()))
            {
                qEntry = std::make_unique<QWebsite>(websiteEntry);
            }
            else if (auto wifiEntry = dynamic_cast<Wifi *>(entry.get()))
            {
                qEntry = std::make_unique<QWifi>(wifiEntry);
            }
            else if (auto creditCardEntry = dynamic_cast<CreditCard *>(entry.get()))
            {
                qEntry = std::make_unique<QCreditCard>(creditCardEntry);
            }
            entries.push_back(std::move(qEntry));
        }
        return entries;
    }
    catch (const CategoryNotFoundError &)
    {
        return {};
    }
}

const QList<QPersona> VaultController::getPersonas() const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }

    QList<QPersona> personas;
    for (const auto &persona : session->getPersonas())
    {
        personas.append(QPersona(persona));
    }
    return personas;
}

void VaultController::addPersona(const QString &firstName, const QString &lastName, const QDate &dateOfBirth,
                                 const QString &address, const QString &phone)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    const qint64 days = dateOfBirth.toJulianDay() - 2440588;
    session->addPersona(std::make_unique<Persona>(firstName.toStdString(), lastName.toStdString(),
                                                  fromUnixMilliseconds(days * 86400000LL), address.toStdString(),
                                                  phone.toStdString()));
}

bool VaultController::removePersona(qint64 personaId)
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

bool VaultController::updatePersona(qint64 personaId, const QString &firstName, const QString &lastName,
                                    const QDate &dateOfBirth, const QString &address, const QString &phone)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        const qint64 days = dateOfBirth.toJulianDay() - 2440588;
        const auto &persona = session->getPersonaById(personaId);
        persona->setFirstName(firstName.toStdString());
        persona->setLastName(lastName.toStdString());
        persona->setDateOfBirth(fromUnixMilliseconds(days * 86400000LL));
        persona->setAddress(address.toStdString());
        persona->setPhone(phone.toStdString());
        return true;
    }
    catch (const PersonaNotFoundError &)
    {
        return false; // Persona not found
    }
}

bool VaultController::linkPersonaToEntry(qint64 personaId, qint64 categoryId, qint64 entryId)
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

bool VaultController::unlinkPersonaFromEntry(qint64 categoryId, qint64 entryId)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        const auto &entry = getEntryByIdFromCategory(categoryId, entryId);
        auto *websiteEntry = dynamic_cast<Website *>(entry.get());
        if (websiteEntry == nullptr)
        {
            return false; // Entry is not of the correct type to unlink a persona
        }
        websiteEntry->setPersona(NO_PERSONA_ID);
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

const QPersona VaultController::getPersonaById(qint64 personaId) const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        return session->getPersonaById(personaId);
    }
    catch (const PersonaNotFoundError &)
    {
        throw std::runtime_error("Persona not found.");
    }
}

bool VaultController::addWebsiteEntry(qint64 categoryId, const QString &title, const QString &username,
                                      const QString &password, const QString &url,
                                      const QString &comments, const QString &notes)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }

    return addEntryToCategory(categoryId, std::make_unique<Website>(notes.toStdString(), title.toStdString(),
                                                                    username.toStdString(), password.toStdString(),
                                                                    url.toStdString(), comments.toStdString()));
}

bool VaultController::addWifiEntry(qint64 categoryId, const QString &networkName, const QString &password,
                                   const QString &notes)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }

    return addEntryToCategory(categoryId, std::make_unique<Wifi>(networkName.toStdString(), password.toStdString(),
                                                                 notes.toStdString()));
}

bool VaultController::addCreditCardEntry(qint64 categoryId, const QString &cardHolderName,
                                         const QString &cardNumber, const QString &expiration,
                                         const QString &securityCode, const QString &notes)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }

    return addEntryToCategory(categoryId, std::make_unique<CreditCard>(cardHolderName.toStdString(), cardNumber.toStdString(),
                                                                       expiration.toStdString(), securityCode.toStdString(),
                                                                       notes.toStdString()));
}

bool VaultController::updateWebsiteEntry(const qint64 categoryId, const qint64 entryId, const QString &username, const QString &password,
                                         const QString &url, const QString &comments, const QString &notes)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        const auto &entry = getEntryByIdFromCategory(categoryId, entryId);
        if (entry == nullptr)
        {
            return false; // Entry not found
        }
        if (auto *website = dynamic_cast<Website *>(entry.get()))
        {
            website->setUsername(username.toStdString());
            website->setPassword(password.toStdString());
            website->setUrl(url.toStdString());
            website->setComments(comments.toStdString());
            website->setNotes(notes.toStdString());
            return true;
        }

        return false;
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

bool VaultController::updateWifiEntry(const qint64 categoryId, const qint64 entryId, const QString &networkName, const QString &password,
                                      const QString &notes)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        const auto &entry = getEntryByIdFromCategory(categoryId, entryId);
        if (entry == nullptr)
        {
            return false; // Entry not found
        }
        if (auto *wifi = dynamic_cast<Wifi *>(entry.get()))
        {
            wifi->setNetworkName(networkName.toStdString());
            wifi->setPassword(password.toStdString());
            wifi->setNotes(notes.toStdString());
            return true;
        }

        return false;
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

bool VaultController::updateCreditCardEntry(const qint64 categoryId, const qint64 entryId, const QString &cardHolderName,
                                            const QString &cardNumber, const QString &expiration,
                                            const QString &securityCode, const QString &notes)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        const auto &entry = getEntryByIdFromCategory(categoryId, entryId);
        if (entry == nullptr)
        {
            return false; // Entry not found
        }
        if (auto *creditCard = dynamic_cast<CreditCard *>(entry.get()))
        {
            creditCard->setCardHolderName(cardHolderName.toStdString());
            creditCard->setCardNumber(cardNumber.toStdString());
            creditCard->setExpiration(expiration.toStdString());
            creditCard->setSecurityCode(securityCode.toStdString());
            creditCard->setNotes(notes.toStdString());
            return true;
        }

        return false;
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

bool VaultController::removeEntryFromCategory(qint64 categoryId, qint64 entryId)
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

std::vector<QWebsite> VaultController::getWebsitesByUrl(const QString &url) const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }

    std::vector<QWebsite> websites;
    for (const auto &website : session->getWebsiteByUrl(url.toStdString()))
    {
        websites.emplace_back(website);
    }
    return websites;
}

const QWebsite VaultController::getWebsiteById(qint64 entryId) const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }

    return QWebsite(session->getWebsiteById(entryId));
}

bool VaultController::setAliasForWebsite(qint64 categoryId, qint64 entryId, const QString &aliasId, const QString &alias)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        session->setAliasForWebsite(categoryId, entryId, aliasId.toStdString(), alias.toStdString());
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

std::vector<std::unique_ptr<QEntry>> VaultController::searchEntriesInCategory(qint64 categoryId, const QString &searchTerm) const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        std::vector<std::unique_ptr<QEntry>> entries;
        for (auto &entry : session->searchEntriesInCategory(categoryId, searchTerm.toStdString()))
        {
            std::unique_ptr<QEntry> qEntry;
            if (auto websiteEntry = dynamic_cast<Website *>(entry))
            {
                qEntry = std::make_unique<QWebsite>(websiteEntry);
            }
            else if (auto wifiEntry = dynamic_cast<Wifi *>(entry))
            {
                qEntry = std::make_unique<QWifi>(wifiEntry);
            }
            else if (auto creditCardEntry = dynamic_cast<CreditCard *>(entry))
            {
                qEntry = std::make_unique<QCreditCard>(creditCardEntry);
            }
            entries.push_back(std::move(qEntry));
        }
        return entries;
    }
    catch (const CategoryNotFoundError &)
    {
        throw std::runtime_error("Category not found.");
    }
}

QString VaultController::vaultFilePath(const QString &vaultName) const
{
    return QDir(QString::fromStdString(pathToVaults)).filePath(sanitizedVaultFileStem(vaultName) + ".kvdb");
}

QString VaultController::sanitizedVaultFileStem(const QString &vaultName) const
{
    QString sanitized = vaultName.toLower();
    for (QChar &c : sanitized)
    {
        if (!c.isLetterOrNumber())
        {
            c = QChar('-');
        }
    }
    return sanitized;
}

bool VaultController::addEntryToCategory(int64_t categoryId, std::unique_ptr<Entry> entry)
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }
    try
    {
        session->addEntryToCategory(categoryId, std::move(entry));
        return true;
    }
    catch (const CategoryNotFoundError &)
    {
        return false; // Category not found
    }
}

const std::unique_ptr<Entry> &VaultController::getEntryByIdFromCategory(int64_t categoryId, int64_t entryId) const
{
    if (session == nullptr)
    {
        throw std::runtime_error("Vault session is not initialized. Please unlock a vault first.");
    }

    const auto &entries = session->getEntriesInCategory(categoryId);
    for (const auto &entry : entries)
    {
        if (entry->getId() == entryId)
        {
            return entry;
        }
    }
    throw EntryNotFoundError("Entry not found.");
}