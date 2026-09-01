#include "nativemessagedispatcher.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QUrl>

#include "model/entryrepository.h"
#include "model/websiteentrydata.h"

namespace {
/**
 * @brief Reduces a host to its registrable domain (last two labels), so sibling subdomains are treated as the same site.
 * @param host The host to reduce.
 * @return The registrable domain in lowercase.
*/
QString registrableDomain(const QString &host)
{
    const QStringList labels = host.split('.', Qt::SkipEmptyParts);

    if (labels.size() < 2) {
        return host.toLower();
    }

    return (labels[labels.size() - 2] + "." + labels[labels.size() - 1]).toLower();
}

/**
 * @brief Compares two hosts for equality based on their registrable domains.
 * @param a The first host to compare.
 * @param b The second host to compare.
 * @return True if the registrable domains of both hosts are equal, false otherwise.
*/
bool hostsMatch(const QString &a, const QString &b)
{
    if (a.isEmpty() || b.isEmpty()) {
        return false;
    }

    return registrableDomain(a) == registrableDomain(b);
}

} // namespace

NativeMessageDispatcher::NativeMessageDispatcher(EntryRepository *repository, QObject *parent)
    : QObject{parent}
    , m_repository{repository}
{}

QByteArray NativeMessageDispatcher::dispatch(const QByteArray &request) const
{
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(request, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return errorResponse("INVALID_JSON");
    }

    const QJsonObject object = doc.object();
    const QString type = object.value("type").toString();

    if (type == "GET_ENTRIES") {
        return handleGetEntries(object);
    }

    if (type == "GET_PASSWORD") {
        return handleGetPassword(object);
    }

    return errorResponse("UNKNOWN_MESSAGE_TYPE");
}

QByteArray NativeMessageDispatcher::handleGetEntries(const QJsonObject &request) const
{
    if (!request.value("url").isString()) {
        return errorResponse("INVALID_REQUEST");
    }

    const QString requestHost = QUrl(request.value("url").toString()).host();

    QJsonArray entriesArray;

    for (const auto &entry : m_repository->entries()) {
        if (entry->type != EntryType::Website) {
            continue;
        }

        const auto *website = static_cast<const WebsiteEntryData *>(entry.get());

        if (!hostsMatch(QUrl(website->url).host(), requestHost)) {
            continue;
        }

        QJsonObject entryObject;
        entryObject["id"] = website->id;
        entryObject["username"] = website->username;
        entriesArray.append(entryObject);
    }

    QJsonObject response;
    response["type"] = "ENTRIES";
    response["entries"] = entriesArray;

    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray NativeMessageDispatcher::handleGetPassword(const QJsonObject &request) const
{
    if (!request.value("id").isString()) {
        return errorResponse("INVALID_REQUEST");
    }

    const QString id = request.value("id").toString();
    const auto entry = m_repository->findById(id);

    if (entry == nullptr || entry->type != EntryType::Website) {
        return errorResponse("NOT_FOUND");
    }

    const auto *website = static_cast<const WebsiteEntryData *>(entry.get());

    QJsonObject response;
    response["type"] = "PASSWORD";
    response["password"] = website->password;

    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray NativeMessageDispatcher::errorResponse(const QString &code)
{
    QJsonObject response;
    response["type"] = "ERROR";
    response["code"] = code;

    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}
