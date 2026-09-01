#pragma once

#include "../src/mailaliasclient.h"

#include <utility>

/**
 * @brief A fake implementation of the MailAliasClient interface for testing purposes.
 * Returns a canned response and records the arguments of the last call.
 */
class FakeMailAliasClient : public MailAliasClient {
public:
    explicit FakeMailAliasClient(std::optional<MailAlias> response, std::string errorMessage = {})
        : response(std::move(response)), errorMessage(std::move(errorMessage)) {}

    std::optional<MailAlias> createAlias(const QString &apiKey, const QString &sourceEmail,
                                          const QString &description, std::string &error) override {
        callCount++;
        lastApiKey = apiKey;
        lastSourceEmail = sourceEmail;
        lastDescription = description;

        if (!response) {
            error = errorMessage;
        }

        return response;
    }

    int callCount = 0;
    QString lastApiKey;
    QString lastSourceEmail;
    QString lastDescription;

private:
    std::optional<MailAlias> response;
    std::string errorMessage;
};
