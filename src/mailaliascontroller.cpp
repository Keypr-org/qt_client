#include "mailaliascontroller.h"

MailAliasController::MailAliasController()
    : config(MailAliasConfig::load()), client(std::make_unique<PostscaleClient>()) {}

MailAliasController::MailAliasController(MailAliasConfig config)
    : config(std::move(config)), client(std::make_unique<PostscaleClient>()) {}

MailAliasController::MailAliasController(MailAliasConfig config, std::unique_ptr<MailAliasClient> client)
    : config(std::move(config)), client(std::move(client)) {}

bool MailAliasController::setCredentials(const std::string &apiKey, const std::string &sourceEmail) {
    config.apiKey = QString::fromStdString(apiKey);
    config.sourceEmail = QString::fromStdString(sourceEmail);
    return config.save();
}

bool MailAliasController::hasCredentials() const {
    return !config.apiKey.isEmpty() && !config.sourceEmail.isEmpty();
}

std::string MailAliasController::apiKey() const {
    return config.apiKey.toStdString();
}

std::string MailAliasController::sourceEmail() const {
    return config.sourceEmail.toStdString();
}

const std::string &MailAliasController::lastError() const {
    return error;
}

std::optional<MailAlias> MailAliasController::createAlias(const std::string &description) {
    if (config.apiKey.isEmpty() || config.sourceEmail.isEmpty()) {
        error = "Missing Postscale API key or source email in configuration";
        return std::nullopt;
    }

    return client->createAlias(config.apiKey, config.sourceEmail, QString::fromStdString(description), error);
}

bool MailAliasController::deleteAlias(const std::string &aliasId) {
    if (config.apiKey.isEmpty()) {
        error = "Missing Postscale API key in configuration";
        return false;
    }

    return client->deleteAlias(config.apiKey, QString::fromStdString(aliasId), error);
}
