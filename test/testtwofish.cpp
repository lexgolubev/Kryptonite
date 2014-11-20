#include <QtTest/QtTest>
#include <iostream>

#include "twofish.h"

 class TestTwofish : public QObject
 {
     Q_OBJECT
 private slots:
     void ror4();
     void ror();
     void rol();
     void test256();
     void test192();
     void mul_gf();
     void q0_q1();
 };

void TestTwofish::ror4() {
    Twofish twofish(0, 0);
    QVERIFY(0x05 == twofish.ror4(0x0a, 1));
    QVERIFY(0x0d == twofish.ror4(0x0b, 1));
    QVERIFY(0x0e == twofish.ror4(0x0b, 2));
}

void TestTwofish::ror() {
    Twofish twofish(0, 0);
    QVERIFY(0xB000069A == twofish.ror(0xd356, 5));
}

void TestTwofish::rol() {
    Twofish twofish(0, 0);
    QVERIFY(0xd356 == twofish.rol(0xB000069A, 5));
}

void TestTwofish::test256() {
    mpz_class key;
    key.set_str("0123456789ABCDEFFEDCBA987654321000112233445566778899AABBCCDDEEFF", 16);
    Twofish twofish(key, 256);

    mpz_class text;
    text.set_str("00000000000000000000000000000000", 16);

    mpz_class encrypted_expected;
    encrypted_expected.set_str("37527BE0052334B89F0CFCCAE87CFA20", 16);
    mpz_class encrypted = twofish.encrypt(text);
    QVERIFY(encrypted_expected == encrypted);

    mpz_class decrypted_expected;
    decrypted_expected.set_str("00000000000000000000000000000000", 16);
    mpz_class decrypted = twofish.decrypt(encrypted);
    QVERIFY(decrypted_expected == decrypted);
}

void TestTwofish::test192() {
    mpz_class key;
    key.set_str("0123456789ABCDEFFEDCBA98765432100011223344556677", 16);
    Twofish twofish(key, 192);

    mpz_class text;
    text.set_str("00000000000000000000000000000000", 16);

    mpz_class encrypted_expected;
    encrypted_expected.set_str("CFD1D2E5A9BE9CDF501F13B892BD2248", 16);
    mpz_class encrypted = twofish.encrypt(text);
    QVERIFY(encrypted_expected == encrypted);

    mpz_class decrypted_expected;
    decrypted_expected.set_str("00000000000000000000000000000000", 16);
    mpz_class decrypted = twofish.decrypt(encrypted);
    QVERIFY(decrypted_expected == decrypted);
}

void TestTwofish::mul_gf() {
    Twofish twofish(0, 0);
    QVERIFY(0x0 == twofish.multiply_gf(0x0, 0x0));
    QVERIFY(0x0 == twofish.multiply_gf(0xf3, 0x0));
    QVERIFY(0xf3 == twofish.multiply_gf(0xf3, 0x1));
    QVERIFY(0x1e6 == twofish.multiply_gf(0xf3, 0x2));
    QVERIFY(0x115 == twofish.multiply_gf(0xf3, 0x3));
    QVERIFY(0x139A == twofish.multiply_gf(0xb9, 0x2a));
    QVERIFY(0xe4 == twofish.mod_gf(0x139a, 0x14d));
}

void TestTwofish::q0_q1() {
    Twofish twofish(0, 0);
    QVERIFY(0xa5 == twofish.q0(0x3a));
    QVERIFY(0x64 == twofish.q1(0xbf));
}

QTEST_MAIN(TestTwofish)
#include "testtwofish.moc"
