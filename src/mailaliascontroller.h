#pragma once

#include "mailaliasclient.h"
#include "mailaliasconfig.h"

#include <memory>
#include <optional>
#include <string>

/**
 * @brief The MailAliasController class creates on-the-fly masked email aliases
 * via the Postscale Shield API, forwarding them to the user's configured source email.
 */
class MailAliasController {
public:
    /**
     * @brief Load the persisted config from disk (see MailAliasConfig::load()).
     */
    MailAliasController();
    explicit MailAliasController(MailAliasConfig config);

    /**
     * @brief Constructs the controller with an explicit config and client, for testing.
     */
    MailAliasController(MailAliasConfig config, std::unique_ptr<MailAliasClient> client);

    /**
     * @brief Update the API key and source email, persisting them to disk.
     * @return true if the config was saved successfully.
     */
    bool setCredentials(const std::string &apiKey, const std::string &sourceEmail);

    /**
     * @brief Whether an API key and source email are currently configured.
     */
    bool hasCredentials() const;

    std::string apiKey() const;
    std::string sourceEmail() const;

    /**
     * @brief Request a new alias forwarding to the configured source email.
     * @param description Label for the alias (e.g. the target website).
     * @return The created alias, or std::nullopt on failure (see lastError()).
     */
    std::optional<MailAlias> createAlias(const std::string &description);

    /**
     * @brief Permanently deletes an existing alias.
     * @param aliasId Identifier of the alias to delete.
     * @return true if the alias was deleted successfully (see lastError() on failure).
     */
    bool deleteAlias(const std::string &aliasId);

    const std::string &lastError() const;

private:
    MailAliasConfig config;
    std::unique_ptr<MailAliasClient> client;
    std::string error;
};
