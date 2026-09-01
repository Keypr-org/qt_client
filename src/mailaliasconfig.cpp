#include "mailaliasconfig.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>

QString MailAliasConfig::configFilePath() {
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(dir);
    return dir + "/mailalias.json";
}

MailAliasConfig MailAliasConfig::fromJson(const QJsonObject &json) {
    MailAliasConfig config;
    config.apiKey = json.value("apiKey").toString();
    config.sourceEmail = json.value("sourceEmail").toString();
    return config;
}

QJsonObject MailAliasConfig::toJson() const {
    QJsonObject json;
    json["apiKey"] = apiKey;
    json["sourceEmail"] = sourceEmail;
    return json;
}

MailAliasConfig MailAliasConfig::load() {
    QFile file(configFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        return MailAliasConfig();
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    return fromJson(doc.object());
}

bool MailAliasConfig::save() const {
    QFile file(configFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return file.write(QJsonDocument(toJson()).toJson()) != -1;
}
