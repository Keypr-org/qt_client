#include "vaultbridge.h"
#include "vaultcontroller.h"
#include "Types.h"
#include "entities/Category.h"
#include "entities/CreditCard.h"
#include "entities/Persona.h"
#include "entities/Website.h"
#include "entities/Wifi.h"

#include <QDir>
#include <QFileInfo>

namespace {
/**
 * @brief Sanitizes a vault name to create a valid filename for the vault file.
 * @param vaultName The original vault name.
 * @return A sanitized string suitable for use as a filename.
 */
QString sanitizedVaultFileStem(const QString &vaultName) {
    QString sanitized = vaultName.toLower();
    for (QChar &c : sanitized) {
        if (!c.isLetterOrNumber()) {
            c = QChar('-');
        }
    }
    return sanitized;
}

/**
 * @brief Finds an entry by its ID across all categories in the given VaultController.
 * @param controller The VaultController to search within.
 * @param entryId The ID of the entry to find.
 * @return A pointer to the Entry if found, or nullptr if not found or if the vault is locked.
 */
Entry *findEntry(VaultController &controller, const QString &entryId) {
    if (!controller.isVaultUnlocked()) {
        return nullptr;
    }

    const int64_t id = entryId.toLongLong();
    for (const auto &category : controller.getCategories()) {
        if (Entry *entry = category->findEntryById(id)) {
            return entry;
        }
    }
    return nullptr;
}

/**
 * @brief Converts a VaultController Entry to a VaultBridge::EntrySummary.
 * @param entry The Entry to convert.
 * @return A VaultBridge::EntrySummary representing the entry.
 */
VaultBridge::EntrySummary toEntrySummary(const Entry *entry) {
    VaultBridge::EntrySummary summary;
    summary.id = QString::number(entry->getId());
    summary.notes = QString::fromStdString(entry->getNotes());
    summary.lastUpdated = QDateTime::fromMSecsSinceEpoch(toUnixMilliseconds(entry->getLastModifiedDate()));

    if (const auto *website = dynamic_cast<const Website *>(entry)) {
        summary.kind = VaultBridge::EntryKind::Website;
        summary.title = QString::fromStdString(website->getTitle());
        summary.username = QString::fromStdString(website->getUsername());
        summary.password = QString::fromStdString(website->getPassword());
        summary.url = QString::fromStdString(website->getUrl());
        summary.comments = QString::fromStdString(website->getComments());
        summary.aliasId = QString::fromStdString(website->getAliasId());
        summary.alias = QString::fromStdString(website->getAlias());
        summary.personaId = website->getPersonaId();
    } else if (const auto *wifi = dynamic_cast<const Wifi *>(entry)) {
        summary.kind = VaultBridge::EntryKind::Wifi;
        summary.networkName = QString::fromStdString(wifi->getNetworkName());
        summary.password = QString::fromStdString(wifi->getPassword());
    } else if (const auto *card = dynamic_cast<const CreditCard *>(entry)) {
        summary.kind = VaultBridge::EntryKind::CreditCard;
        summary.cardHolderName = QString::fromStdString(card->getCardHolderName());
        summary.cardNumber = QString::fromStdString(card->getCardNumber());
        summary.expiration = QString::fromStdString(card->getExpiration());
        summary.securityCode = QString::fromStdString(card->getSecurityCode());
    }

    return summary;
}

constexpr qint64 UnixEpochJulianDay = 2440588;

DateTime qDateToDateTime(const QDate &date) {
    const qint64 days = date.toJulianDay() - UnixEpochJulianDay;
    return fromUnixMilliseconds(days * 86400000LL);
}

QDate dateTimeToQDate(DateTime dateTime) {
    const qint64 days = toUnixMilliseconds(dateTime) / 86400000LL;
    return QDate::fromJulianDay(UnixEpochJulianDay + days);
}
}

VaultBridge::VaultBridge(const QString &vaultStoragePath)
    : m_vaultStoragePath(vaultStoragePath)
    , m_controller(std::make_unique<VaultController>(nullptr, vaultStoragePath.toStdString())) {}

VaultBridge::~VaultBridge() = default;

QString VaultBridge::vaultFilePath(const QString &vaultName) const {
    return QDir(m_vaultStoragePath).filePath(sanitizedVaultFileStem(vaultName) + ".kvdb");
}

bool VaultBridge::createVault(const QString &vaultName, const QString &masterPassword) {
    if (!m_controller->createVault(masterPassword.toStdString(), vaultName.toStdString())) {
        return false;
    }
    return m_controller->lockVault();
}

VaultBridge::UnlockResult VaultBridge::unlockVault(const QString &vaultName, const QString &masterPassword) {
    const QString path = vaultFilePath(vaultName);
    if (!QFileInfo::exists(path)) {
        return UnlockResult::VaultFileNotFound;
    }

    if (!m_controller->unlockVault(masterPassword.toStdString(), path.toStdString())) {
        return UnlockResult::IncorrectPasswordOrCorrupted;
    }

    return UnlockResult::Success;
}

bool VaultBridge::lockVault() {
    if (!m_controller->isVaultUnlocked()) {
        return false;
    }
    return m_controller->lockVault();
}

QList<VaultBridge::CategorySummary> VaultBridge::categories() const {
    QList<CategorySummary> result;
    if (!m_controller->isVaultUnlocked()) {
        return result;
    }

    for (const auto &category : m_controller->getCategories()) {
        result.append({category->getId(), QString::fromStdString(category->getName())});
    }
    return result;
}

bool VaultBridge::addCategory(const QString &name) {
    if (!m_controller->isVaultUnlocked()) {
        return false;
    }

    m_controller->addCategory(std::make_unique<Category>(name.toStdString()));
    return true;
}

QList<VaultBridge::EntrySummary> VaultBridge::entriesInCategory(qint64 categoryId) const {
    QList<EntrySummary> result;
    if (!m_controller->isVaultUnlocked()) {
        return result;
    }

    for (const auto &entry : m_controller->getEntriesInCategory(categoryId)) {
        result.append(toEntrySummary(entry.get()));
    }
    return result;
}

QList<VaultBridge::EntrySummary> VaultBridge::searchEntriesInCategory(qint64 categoryId, const QString &searchTerm) const {
    QList<EntrySummary> result;
    if (!m_controller->isVaultUnlocked()) {
        return result;
    }

    try {
        for (const Entry *entry : m_controller->searchEntriesInCategory(categoryId, searchTerm.toStdString())) {
            result.append(toEntrySummary(entry));
        }
    } catch (const std::runtime_error &) {
        // e.g. the category no longer exists.
    }
    return result;
}

QString VaultBridge::addWebsiteEntry(qint64 categoryId, const QString &title, const QString &username,
                                      const QString &password, const QString &url,
                                      const QString &comments, const QString &notes) {
    if (!m_controller->isVaultUnlocked()) {
        return QString();
    }

    auto website = std::make_unique<Website>(notes.toStdString(), title.toStdString(),
                                              username.toStdString(), password.toStdString(),
                                              url.toStdString(), comments.toStdString());
    const QString id = QString::number(website->getId());
    if (!m_controller->addEntryToCategory(categoryId, std::move(website))) {
        return QString();
    }
    return id;
}

QString VaultBridge::addWifiEntry(qint64 categoryId, const QString &networkName,
                                   const QString &password, const QString &notes) {
    if (!m_controller->isVaultUnlocked()) {
        return QString();
    }

    auto wifi = std::make_unique<Wifi>(networkName.toStdString(), password.toStdString(),
                                       notes.toStdString());
    const QString id = QString::number(wifi->getId());
    if (!m_controller->addEntryToCategory(categoryId, std::move(wifi))) {
        return QString();
    }
    return id;
}

QString VaultBridge::addCreditCardEntry(qint64 categoryId, const QString &cardHolderName,
                                         const QString &cardNumber, const QString &expiration,
                                         const QString &securityCode, const QString &notes) {
    if (!m_controller->isVaultUnlocked()) {
        return QString();
    }

    auto card = std::make_unique<CreditCard>(cardHolderName.toStdString(), cardNumber.toStdString(),
                                             expiration.toStdString(), securityCode.toStdString(),
                                             notes.toStdString());
    const QString id = QString::number(card->getId());
    if (!m_controller->addEntryToCategory(categoryId, std::move(card))) {
        return QString();
    }
    return id;
}

bool VaultBridge::removeEntry(qint64 categoryId, const QString &entryId) {
    if (!m_controller->isVaultUnlocked()) {
        return false;
    }
    return m_controller->removeEntryFromCategory(categoryId, entryId.toLongLong());
}

bool VaultBridge::updateWebsiteEntry(const QString &entryId, const QString &username,
                                      const QString &password, const QString &url,
                                      const QString &comments, const QString &notes) {
    auto *website = dynamic_cast<Website *>(findEntry(*m_controller, entryId));
    if (!website) {
        return false;
    }

    website->setUsername(username.toStdString());
    website->setPassword(password.toStdString());
    website->setUrl(url.toStdString());
    website->setComments(comments.toStdString());
    website->setNotes(notes.toStdString());
    return true;
}

bool VaultBridge::updateWifiEntry(const QString &entryId, const QString &networkName,
                                   const QString &password, const QString &notes) {
    auto *wifi = dynamic_cast<Wifi *>(findEntry(*m_controller, entryId));
    if (!wifi) {
        return false;
    }

    wifi->setNetworkName(networkName.toStdString());
    wifi->setPassword(password.toStdString());
    wifi->setNotes(notes.toStdString());
    return true;
}

bool VaultBridge::updateCreditCardEntry(const QString &entryId, const QString &cardHolderName,
                                         const QString &cardNumber, const QString &expiration,
                                         const QString &securityCode, const QString &notes) {
    auto *card = dynamic_cast<CreditCard *>(findEntry(*m_controller, entryId));
    if (!card) {
        return false;
    }

    card->setCardHolderName(cardHolderName.toStdString());
    card->setCardNumber(cardNumber.toStdString());
    card->setExpiration(expiration.toStdString());
    card->setSecurityCode(securityCode.toStdString());
    card->setNotes(notes.toStdString());
    return true;
}

bool VaultBridge::linkPersonaToWebsite(qint64 categoryId, const QString &entryId, qint64 personaId) {
    if (!m_controller->isVaultUnlocked()) {
        return false;
    }
    return m_controller->linkPersonaToEntry(personaId, categoryId, entryId.toLongLong());
}

bool VaultBridge::unlinkPersonaFromWebsite(const QString &entryId) {
    auto *website = dynamic_cast<Website *>(findEntry(*m_controller, entryId));
    if (!website) {
        return false;
    }
    website->setPersona(NO_PERSONA_ID);
    return true;
}

bool VaultBridge::setWebsiteAlias(const QString &entryId, const QString &aliasId, const QString &alias) {
    auto *website = dynamic_cast<Website *>(findEntry(*m_controller, entryId));
    if (!website) {
        return false;
    }
    website->setAliasId(aliasId.toStdString());
    website->setAlias(alias.toStdString());
    return true;
}

QList<VaultBridge::PersonaSummary> VaultBridge::personas() const {
    QList<PersonaSummary> result;
    if (!m_controller->isVaultUnlocked()) {
        return result;
    }

    for (const auto &persona : m_controller->getPersonas()) {
        PersonaSummary summary;
        summary.id = persona->getId();
        summary.firstName = QString::fromStdString(persona->getFirstName());
        summary.lastName = QString::fromStdString(persona->getLastName());
        summary.dateOfBirth = dateTimeToQDate(persona->getDateOfBirth());
        summary.address = QString::fromStdString(persona->getAddress());
        summary.phone = QString::fromStdString(persona->getPhone());
        result.append(summary);
    }
    return result;
}

qint64 VaultBridge::addPersona(const QString &firstName, const QString &lastName,
                                const QDate &dateOfBirth, const QString &address,
                                const QString &phone) {
    if (!m_controller->isVaultUnlocked()) {
        return -1;
    }

    auto persona = std::make_unique<Persona>(firstName.toStdString(), lastName.toStdString(),
                                             qDateToDateTime(dateOfBirth), address.toStdString(),
                                             phone.toStdString());
    const qint64 id = persona->getId();
    m_controller->addPersona(std::move(persona));
    return id;
}

bool VaultBridge::updatePersona(qint64 personaId, const QString &firstName, const QString &lastName,
                                 const QDate &dateOfBirth, const QString &address,
                                 const QString &phone) {
    if (!m_controller->isVaultUnlocked()) {
        return false;
    }

    try {
        const auto &persona = m_controller->getPersonaById(personaId);
        persona->setFirstName(firstName.toStdString());
        persona->setLastName(lastName.toStdString());
        persona->setDateOfBirth(qDateToDateTime(dateOfBirth));
        persona->setAddress(address.toStdString());
        persona->setPhone(phone.toStdString());
        return true;
    } catch (const std::runtime_error &) {
        return false;
    }
}

bool VaultBridge::removePersona(qint64 personaId) {
    if (!m_controller->isVaultUnlocked()) {
        return false;
    }
    return m_controller->removePersona(personaId);
}
