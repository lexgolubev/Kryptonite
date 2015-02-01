#include <QtTest/QtTest>
#include <iostream>
#include "../algo/twofish/twofish.h"
#include "../algo/bytearray.h"

class TestTwofish : public QObject
{
    Q_OBJECT
private slots:
    void test256();
    void test192();
    void test192_2();
    void testMpz();
    void testByteArrayLittleEndian();
    void testEncryptQstr();
};

void TestTwofish::test256() {
    mpz_class key;
    key.set_str("0123456789ABCDEFFEDCBA987654321000112233445566778899AABBCCDDEEFF", 16);
    Twofish twofish(key, 256);

    mpz_class text;
    text.set_str("00000000000000000000000000000000", 16);

    mpz_class encrypted_expected;
    encrypted_expected.set_str("37527BE0052334B89F0CFCCAE87CFA20", 16);
    mpz_class encrypted = twofish.encrypt_mpz(text);
    QVERIFY(encrypted_expected == encrypted);

    mpz_class decrypted = twofish.decrypt_mpz(encrypted);
    QVERIFY(text == decrypted);
}

void TestTwofish::test192() {
    mpz_class key;
    key.set_str("0123456789ABCDEFFEDCBA98765432100011223344556677", 16);
    Twofish twofish(key, 192);

    mpz_class text;
    text.set_str("00000000000000000000000000000000", 16);

    mpz_class encrypted_expected;
    encrypted_expected.set_str("CFD1D2E5A9BE9CDF501F13B892BD2248", 16);
    mpz_class encrypted = twofish.encrypt_mpz(text);
    QVERIFY(encrypted_expected == encrypted);

    mpz_class decrypted = twofish.decrypt_mpz(encrypted);
    QVERIFY(text == decrypted);
}

void TestTwofish::test192_2() {
    mpz_class key;
    key.set_str("EFA71F788965BD4453F860178FC191010000000000000000", 16);
    Twofish twofish(key, 192);

    mpz_class text;
    text.set_str("88B2B2706B105E36B446BB6D731A1E88", 16);

    mpz_class encrypted_expected;
    encrypted_expected.set_str("39DA69D6BA4997D585B6DC073CA341B2", 16);
    mpz_class encrypted = twofish.encrypt_mpz(text);
    QVERIFY(encrypted_expected == encrypted);

    mpz_class decrypted = twofish.decrypt_mpz(encrypted);
    QVERIFY(text == decrypted);
}

void TestTwofish::testMpz() {
    mpz_class key;
    key.set_str("0123456789ABCDEFFEDCBA987654321000112233445566778899AABBCCDDEEFF", 16);
    Twofish twofish(key, 256);

    char str[] = "hello123  678901";

    mpz_class encrypted_expected;
    encrypted_expected.set_str("37527BE0052334B89F0CFCCAE87CFA20", 16);
    mpz_class encrypted = twofish.encrypt_str(str);

    qDebug() << "encr" << encrypted.get_str(16).c_str();
//    QVERIFY(encrypted_expected == encrypted);

    char* decrypted = twofish.decrypt_str(encrypted);
//    QVERIFY(text == decrypted);

    qDebug() << "res = " << decrypted;
}

void TestTwofish::testByteArrayLittleEndian() {
    ByteArray arr("12345678");
    arr.toLittleEndian();
    qDebug() << arr;
    qDebug() << (char*)arr;
}

void TestTwofish::testEncryptQstr() {
    mpz_class key;
    key.set_str("EFA71F788965BD4453F860178FC191010000000000000000", 16);
    Twofish twofish(key, 192);

    QString text = "";

    QString encrypted_expected = "37527BE0052334B89F0CFCCAE87CFA20";
    QString encrypted = twofish.encrypt_qstr(text);
    qDebug() << "encrypted:" << encrypted;
    QVERIFY(encrypted_expected == encrypted);

    //mpz_class decrypted = twofish.decrypt_mpz(encrypted);
    //QVERIFY(text == decrypted);
}

QTEST_MAIN(TestTwofish)
#include "testtwofish.moc"
