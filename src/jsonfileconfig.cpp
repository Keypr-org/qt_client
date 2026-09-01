#include "jsonfileconfig.h"

#include <QFile>
#include <QJsonDocument>

QJsonObject JsonFileConfig::read(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QJsonObject();
    }
    return QJsonDocument::fromJson(file.readAll()).object();
}

bool JsonFileConfig::write(const QString &path, const QJsonObject &json) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return file.write(QJsonDocument(json).toJson()) != -1;
}
