#include "mailaliasclient.h"

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

namespace {
const char *POSTSCALE_ALIASES_URL = "https://api.postscale.io/v1/shield/aliases";
}

std::optional<MailAlias> PostscaleClient::createAlias(const QString &apiKey, const QString &sourceEmail,
                                                        const QString &description, std::string &error) {
    QJsonObject body;
    body["forward_to"] = QJsonArray{sourceEmail};
    body["description"] = description;

    QNetworkRequest request{QUrl(POSTSCALE_ALIASES_URL)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());

    QNetworkAccessManager manager;
    QEventLoop loop;
    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkReply *reply = manager.post(request, QJsonDocument(body).toJson(QJsonDocument::Compact));
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        error = reply->errorString().toStdString();
        reply->deleteLater();
        return std::nullopt;
    }

    const QByteArray rawResponse = reply->readAll();
    reply->deleteLater();

    const QJsonObject aliasJson = QJsonDocument::fromJson(rawResponse).object().value("alias").toObject();

    MailAlias alias;
    alias.id = aliasJson.value("id").toString().toStdString();
    alias.address = aliasJson.value("address").toString().toStdString();
    alias.domain = aliasJson.value("domain").toString().toStdString();
    alias.active = aliasJson.value("active").toBool();
    for (const QJsonValue &value : aliasJson.value("forward_to").toArray()) {
        alias.forwardTo.push_back(value.toString().toStdString());
    }

    if (alias.address.empty() || alias.domain.empty()) {
        error = "Unexpected response from Postscale API: " + std::string(rawResponse.constData(), rawResponse.size());
        return std::nullopt;
    }

    return alias;
}

bool PostscaleClient::deleteAlias(const QString &apiKey, const QString &aliasId, std::string &error) {
    QNetworkRequest request{QUrl(QString("%1/%2").arg(POSTSCALE_ALIASES_URL, aliasId))};
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());

    QNetworkAccessManager manager;
    QEventLoop loop;
    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkReply *reply = manager.deleteResource(request);
    loop.exec();

    const bool succeeded = reply->error() == QNetworkReply::NoError;
    if (!succeeded) {
        error = reply->errorString().toStdString();
    }
    reply->deleteLater();

    return succeeded;
}
