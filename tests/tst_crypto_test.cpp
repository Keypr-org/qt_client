#include <QtTest/QtTest>
#include "CryptoService.h"
#include <QString>

class TestCrypto : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testRandomBytes();
    void testRandomBytesUnique();
    void testEncryptDecrypt();
    void testEncryptDecryptMultiple();
    void testInvalidKeyLength();
    void testDecryptTampered();
    void testHashPassword();
    void testHashPasswordUnique();

private:
    CryptoService* crypto;
};

void TestCrypto::initTestCase()
{
    crypto = new CryptoService();
}

void TestCrypto::testRandomBytes()
{
    std::string random = crypto->generateRandomBytes(32);
    QCOMPARE(random.length(), size_t(32));
}

void TestCrypto::testRandomBytesUnique()
{
    std::string random1 = crypto->generateRandomBytes(32);
    std::string random2 = crypto->generateRandomBytes(32);
    QVERIFY(random1 != random2);
}

void TestCrypto::testEncryptDecrypt()
{
    std::string key = crypto->generateRandomBytes(32);
    std::string plaintext = "Hello, libsodium!";

    std::string ciphertext = crypto->encrypt(plaintext, key);
    QVERIFY(!ciphertext.empty());
    QVERIFY(ciphertext != plaintext);

    std::string decrypted = crypto->decrypt(ciphertext, key);
    QCOMPARE(QString::fromStdString(decrypted), QString::fromStdString(plaintext));
}

void TestCrypto::testEncryptDecryptMultiple()
{
    std::string key = crypto->generateRandomBytes(32);
    std::string plaintext1 = "Message 1";
    std::string plaintext2 = "Message 2";

    std::string cipher1 = crypto->encrypt(plaintext1, key);
    std::string cipher2 = crypto->encrypt(plaintext2, key);

    // Même avec la même clé, les chiffrés sont différents (à cause du nonce aléatoire)
    QVERIFY(cipher1 != cipher2);

    QCOMPARE(QString::fromStdString(crypto->decrypt(cipher1, key)),
             QString::fromStdString(plaintext1));
    QCOMPARE(QString::fromStdString(crypto->decrypt(cipher2, key)),
             QString::fromStdString(plaintext2));
}

void TestCrypto::testInvalidKeyLength()
{
    std::string wrongKey = "tooshort";
    std::string plaintext = "test";

    QVERIFY_EXCEPTION_THROWN(crypto->encrypt(plaintext, wrongKey), std::runtime_error);
}

void TestCrypto::testDecryptTampered()
{
    std::string key = crypto->generateRandomBytes(32);
    std::string plaintext = "Secret message";

    std::string ciphertext = crypto->encrypt(plaintext, key);

    // Modifier un caractère du ciphertext
    if (ciphertext.length() > 10) {
        ciphertext[10] = (ciphertext[10] == 'a') ? 'b' : 'a';
    }

    QVERIFY_EXCEPTION_THROWN(crypto->decrypt(ciphertext, key), std::runtime_error);
}

void TestCrypto::testHashPassword()
{
    std::string password = "MySecurePassword123";
    std::string hash = crypto->hashPassword(password);
    QVERIFY(!hash.empty());
}

void TestCrypto::testHashPasswordUnique()
{
    std::string password = "MySecurePassword123";
    std::string hash1 = crypto->hashPassword(password);
    std::string hash2 = crypto->hashPassword(password);

    // Deux hashes du même password doivent être différents (salt aléatoire)
    QVERIFY(hash1 != hash2);
}

QTEST_MAIN(TestCrypto)
#include "tst_crypto_test.moc"