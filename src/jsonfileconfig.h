#pragma once

#include <QJsonObject>
#include <QString>

/**
 * @brief Shared read/write helpers for the small JSON config files persisted under
 * QStandardPaths::AppConfigLocation (see AppConfig, MailAliasConfig).
 */
namespace JsonFileConfig {
    /**
     * @brief Reads a JSON file from the given path and returns its contents as a QJsonObject.
     * @param path The path to the JSON file.
     * @return The QJsonObject representing the contents of the JSON file.
     */
    QJsonObject read(const QString &path);
    
    /**
     * @brief Writes a QJsonObject to a JSON file at the given path.
     * @param path The path to the JSON file.
     * @param json The QJsonObject to write.
     * @return true if the write was successful, false otherwise.
     */
    bool write(const QString &path, const QJsonObject &json);
}
