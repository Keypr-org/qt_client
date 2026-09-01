#pragma once

#include <QJsonObject>
#include <QString>

/**
 * @brief Persisted configuration for the mail alias feature (Postscale API key, source email to forward aliases to).
 */
class MailAliasConfig {
public:
    QString apiKey;
    QString sourceEmail;

    static QString configFilePath();
    static MailAliasConfig load();
    bool save() const;

private:
    static MailAliasConfig fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};
