#include "appconfig.h"
#include "jsonfileconfig.h"

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

QString AppConfig::configFilePath() {
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(dir);
    return dir + "/appconfig.json";
}

AppConfig AppConfig::fromJson(const QJsonObject &json) {
    AppConfig config;
    config.vaultStoragePath = json.value("vaultStoragePath").toString();
    config.apiKey = json.value("apiKey").toString();
    config.sourceEmail = json.value("sourceEmail").toString();
    return config;
}

QJsonObject AppConfig::toJson() const {
    QJsonObject json;
    json["vaultStoragePath"] = vaultStoragePath;
    json["apiKey"] = apiKey;
    json["sourceEmail"] = sourceEmail;
    return json;
}

AppConfig AppConfig::load() {
    return fromJson(JsonFileConfig::read(configFilePath()));
}

bool AppConfig::save() const {
    return JsonFileConfig::write(configFilePath(), toJson());
}

bool AppConfig::hasValidVaultStoragePath() const {
    return !vaultStoragePath.isEmpty() && QFileInfo(vaultStoragePath).isDir();
}
