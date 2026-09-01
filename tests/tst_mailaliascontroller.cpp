#include <QtTest/QtTest>
#include <memory>
#include "fakemailaliasclient.h"
#include "../src/mailaliascontroller.h"

class MailAliasControllerTest : public QObject {
    Q_OBJECT

private slots:
    void hasCredentials_returnsFalse_whenNothingConfigured();
    void createAlias_returnsNullopt_whenCredentialsMissing();
    void createAlias_returnsAlias_whenCredentialsProvidedAndClientSucceeds();
    void createAlias_forwardsSourceEmailAndDescriptionToClient();
    void createAlias_returnsNulloptAndSetsError_whenClientFails();
};

/**
 * @brief hasCredentials() should be false when the config has no API key or source email.
 */
void MailAliasControllerTest::hasCredentials_returnsFalse_whenNothingConfigured() {
    MailAliasController controller(MailAliasConfig{}, std::make_unique<FakeMailAliasClient>(std::nullopt));

    QVERIFY(!controller.hasCredentials());
}

/**
 * @brief createAlias() should fail without ever calling the client when credentials are missing.
 */
void MailAliasControllerTest::createAlias_returnsNullopt_whenCredentialsMissing() {
    auto fakeClient = std::make_unique<FakeMailAliasClient>(std::nullopt);
    FakeMailAliasClient *fakeClientPtr = fakeClient.get();

    MailAliasController controller(MailAliasConfig{}, std::move(fakeClient));

    QVERIFY(!controller.createAlias("some-site").has_value());
    QCOMPARE(fakeClientPtr->callCount, 0);
    QVERIFY(!controller.lastError().empty());
}

/**
 * @brief createAlias() should return the alias produced by the client when all data is provided.
 */
void MailAliasControllerTest::createAlias_returnsAlias_whenCredentialsProvidedAndClientSucceeds() {
    MailAliasConfig config;
    config.apiKey = "ps_live_test";
    config.sourceEmail = "user@example.com";

    MailAlias expectedAlias;
    expectedAlias.address = "xk7m9p";
    expectedAlias.domain = "shield.postscale.io";
    expectedAlias.forwardTo = {"user@example.com"};
    expectedAlias.active = true;

    MailAliasController controller(config, std::make_unique<FakeMailAliasClient>(expectedAlias));

    const auto alias = controller.createAlias("newsletter-signup");

    QVERIFY(alias.has_value());
    QCOMPARE(QString::fromStdString(alias->fullAddress()), QString("xk7m9p@shield.postscale.io"));
    QVERIFY(alias->active);
}

/**
 * @brief createAlias() should forward the configured API key, source email and description to the client.
 */
void MailAliasControllerTest::createAlias_forwardsSourceEmailAndDescriptionToClient() {
    MailAliasConfig config;
    config.apiKey = "ps_live_test";
    config.sourceEmail = "user@example.com";

    MailAlias alias;
    alias.address = "xk7m9p";
    alias.domain = "shield.postscale.io";

    auto fakeClient = std::make_unique<FakeMailAliasClient>(alias);
    FakeMailAliasClient *fakeClientPtr = fakeClient.get();

    MailAliasController controller(config, std::move(fakeClient));
    controller.createAlias("newsletter-signup");

    QCOMPARE(fakeClientPtr->callCount, 1);
    QCOMPARE(fakeClientPtr->lastApiKey, QString("ps_live_test"));
    QCOMPARE(fakeClientPtr->lastSourceEmail, QString("user@example.com"));
    QCOMPARE(fakeClientPtr->lastDescription, QString("newsletter-signup"));
}

/**
 * @brief createAlias() should surface the client's error message and return nullopt on failure.
 */
void MailAliasControllerTest::createAlias_returnsNulloptAndSetsError_whenClientFails() {
    MailAliasConfig config;
    config.apiKey = "ps_live_test";
    config.sourceEmail = "user@example.com";

    MailAliasController controller(config, std::make_unique<FakeMailAliasClient>(std::nullopt, "boom"));

    const auto alias = controller.createAlias("newsletter-signup");

    QVERIFY(!alias.has_value());
    QCOMPARE(QString::fromStdString(controller.lastError()), QString("boom"));
}

QTEST_MAIN(MailAliasControllerTest)

#include "tst_mailaliascontroller.moc"
