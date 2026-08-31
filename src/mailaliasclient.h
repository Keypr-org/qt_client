#pragma once

#include <QString>
#include <optional>
#include <string>
#include <vector>

/**
 * @brief An email alias created through the Postscale Shield API.
 */
struct MailAlias {
    std::string address;
    std::string domain;
    std::vector<std::string> forwardTo;
    bool active = false;

    std::string fullAddress() const { return address + "@" + domain; }
};

/**
 * @brief Abstraction over the remote service used to create mail aliases, so
 * MailAliasController can be tested without making real network calls.
 */
class MailAliasClient {
public:
    virtual ~MailAliasClient() = default;

    /**
     * @brief Requests a new alias forwarding to sourceEmail.
     * @param apiKey Postscale API key.
     * @param sourceEmail Email address the alias should forward to.
     * @param description Label for the alias (e.g. the target website).
     * @param error Set to a human-readable message on failure.
     * @return The created alias, or std::nullopt on failure.
     */
    virtual std::optional<MailAlias> createAlias(const QString &apiKey, const QString &sourceEmail,
                                                  const QString &description, std::string &error) = 0;
};

/**
 * @brief MailAliasClient implementation backed by the real Postscale Shield API.
 */
class PostscaleClient : public MailAliasClient {
public:
    std::optional<MailAlias> createAlias(const QString &apiKey, const QString &sourceEmail,
                                          const QString &description, std::string &error) override;
};
