#pragma once

#include <QJsonObject>
#include <QString>

/**
 * @brief Persisted app-wide configuration: the directory where vault files are
 * stored, and the Postscale API key / source email used for mail aliases.
 */
class AppConfig {
public:
    QString vaultStoragePath;
    QString apiKey;
    QString sourceEmail;

    /**
     * @brief Returns the path to the JSON file where the app config is persisted.
     */
    static QString configFilePath();
    
    /**
     * @brief Loads the app config from the JSON file, or returns a default-constructed
     * AppConfig if the file does not exist or cannot be read.
     */
    static AppConfig load();

    /**
     * @brief Saves the app config to the JSON file.
     * @return true if the save was successful, false otherwise.
     */
    bool save() const;

    /**
     * @brief Whether vaultStoragePath is set and points to an existing directory.
     */
    bool hasValidVaultStoragePath() const;

private:
    static AppConfig fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};
