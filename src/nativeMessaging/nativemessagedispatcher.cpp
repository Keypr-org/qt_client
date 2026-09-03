#include "nativemessagedispatcher.h"
#include "vaultcontroller.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QUrl>

NativeMessageDispatcher::NativeMessageDispatcher(QObject *parent)
    : QObject{parent} {}

QByteArray NativeMessageDispatcher::dispatch(const QByteArray &request) const
{
  QJsonParseError parseError;
  const QJsonDocument doc = QJsonDocument::fromJson(request, &parseError);

  if (parseError.error != QJsonParseError::NoError || !doc.isObject())
  {
    return errorResponse("Request JSON is invalid");
  }

  const QJsonObject object = doc.object();
  const QString type = object.value("type").toString();

  if (type == "GET_ENTRIES")
  {
    return handleGetEntries(object);
  }

  if (type == "GET_PASSWORD")
  {
    return handleGetPassword(object);
  }

  return errorResponse("Unknown message type");
}

QByteArray
NativeMessageDispatcher::handleGetEntries(const QJsonObject &request) const
{
  if (!request.value("url").isString())
  {
    return errorResponse("Invalid Request");
  }

  const QString requestHost = QUrl(request.value("url").toString()).host();

  const VaultController &controller = VaultController::getInstance();

  QJsonArray entriesArray;

  try
  {
    auto entries = controller.getWebsitesByUrl(requestHost);

    for (const auto &websiteEntry : entries)
    {
      QJsonObject entryObject;
      entryObject["id"] = QString::number(websiteEntry.getId());
      entryObject["username"] = websiteEntry.getUsername();
      entriesArray.append(entryObject);
    }

    QJsonObject response;
    response["type"] = "ENTRIES";
    response["entries"] = entriesArray;

    return QJsonDocument(response).toJson(QJsonDocument::Compact);
  }
  catch (VaultNotUnlockedError &e)
  {
    return errorResponse("No vault unlocked, please unlock a vault first");
  }
  catch (std::exception &e)
  {
    return errorResponse("Failed to get entries from the Vault");
  }
}

QByteArray
NativeMessageDispatcher::handleGetPassword(const QJsonObject &request) const
{
  if (!request.value("id").isString())
  {
    return errorResponse("Invalid Request");
  }

  bool ok = false;
  const qint64 id = request.value("id").toString().toLongLong(&ok);

  if (!ok)
  {
    return errorResponse("Invalid ID");
  }

  const VaultController &controller = VaultController::getInstance();

  try
  {
    const QWebsite websiteEntry = controller.getWebsiteById(id);
    QJsonObject response;
    response["type"] = "PASSWORD";
    response["password"] = websiteEntry.getPassword();

    return QJsonDocument(response).toJson(QJsonDocument::Compact);
  }
  catch (VaultNotUnlockedError &e)
  {
    return errorResponse("No vault unlocked, please unlock a vault first");
  }
  catch (std::exception &e)
  {
    return errorResponse("No password matching entry id found: " +
                         request.value("id").toVariant().toString());
  }
}

QByteArray NativeMessageDispatcher::errorResponse(const QString &code)
{
  QJsonObject response;
  response["type"] = "ERROR";
  response["code"] = code;

  return QJsonDocument(response).toJson(QJsonDocument::Compact);
}
