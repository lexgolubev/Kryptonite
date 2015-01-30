#ifndef TWOFISH_H
#define TWOFISH_H

#include <QtGlobal>
#include <gmpxx.h>
#include <utility>
#include <boost/random/random_device.hpp>
#include <bytearray.h>

class Twofish
{
private:
    quint32 keys[40];
    quint32 s[4];
    int k;
private:
    static constexpr quint8 RS_MATRIX[4][8] = {
        {0x01, 0xa4, 0x55, 0x87, 0x5a, 0x58, 0xdb, 0x9e},
        {0xa4, 0x56, 0x82, 0xf3, 0x1e, 0xc6, 0x68, 0xe5},
        {0x02, 0xa1, 0xfc, 0xc1, 0x47, 0xae, 0x3d, 0x19},
        {0xa4, 0x55, 0x87, 0x5a, 0x58, 0xdb, 0x9e, 0x03}
    };
    static constexpr quint16 RS_VECTOR = 0x14d;
    static constexpr quint8 MDS_MATRIX[4][4] = {
        {0x1, 0xef, 0x5b, 0x5b},
        {0x5b, 0xef, 0xef, 0x1},
        {0xef, 0x5b, 0x1, 0xef},
        {0xef, 0x1, 0xef, 0x5b}
    };
    static constexpr quint16 MDS_VECTOR = 0x169;
    static constexpr quint8 Q0[4][16] = {
        {0x8, 0x1, 0x7, 0xD, 0x6, 0xF, 0x3, 0x2, 0x0, 0xB, 0x5, 0x9, 0xE, 0xC, 0xA, 0x4},
        {0xE, 0xC, 0xB, 0x8, 0x1, 0x2, 0x3, 0x5, 0xF, 0x4, 0xA, 0x6, 0x7, 0x0, 0x9, 0xD},
        {0xB, 0xA, 0x5, 0xE, 0x6, 0xD, 0x9, 0x0, 0xC, 0x8, 0xF, 0x3, 0x2, 0x4, 0x7, 0x1},
        {0xD, 0x7, 0xF, 0x4, 0x1, 0x2, 0x6, 0xE, 0x9, 0xB, 0x3, 0x0, 0x8, 0x5, 0xC, 0xA}
    };
    static constexpr quint8 Q1[4][16] = {
        {0x2, 0x8, 0xB, 0xD, 0xF, 0x7, 0x6, 0xE, 0x3, 0x1, 0x9, 0x4, 0x0, 0xA, 0xC, 0x5},
        {0x1, 0xE, 0x2, 0xB, 0x4, 0xC, 0x3, 0x7, 0x6, 0xD, 0xA, 0x5, 0xF, 0x9, 0x0, 0x8},
        {0x4, 0xC, 0x7, 0x5, 0x1, 0x6, 0x9, 0xA, 0x0, 0xE, 0xD, 0x8, 0x2, 0xB, 0x3, 0xF},
        {0xB, 0x9, 0x5, 0x1, 0xC, 0x3, 0xD, 0xE, 0x6, 0x4, 0x7, 0xF, 0x2, 0x0, 0x8, 0xA}
    };
private:
    void key_shedule(const mpz_class& key, int key_length);

    std::pair<quint32, quint32> f(quint32 r0, quint32 r1, int round);
    quint32 g(quint32 word);
    quint32 h(quint32 x, quint32* l, int key);
    quint8 q0_q1_permutation(quint8 word, bool q0);
    quint8 q0(quint8 word);
    quint8 q1(quint8 word);

    static quint16 multiply_gf(quint8 first, quint8 second);
    static quint8 mod_gf(quint16 first, quint16 second);
    template<size_t N, size_t M>
    static quint8* multiply_gf(const quint8 (&matrix)[N][M], const quint8* vector, quint16 base);

    static quint8* split_32_8_reverse(quint32 word);
    static quint32* split_mpz_32_little_endian(const mpz_class& word, int key_length);
    static quint8* split_mpz_8(const mpz_class& word, int key_length);

    static quint8 ror4(quint8 word, int bits);
    static quint32 ror(quint32 word, int bits);
    static quint32 rol(quint32 word, int bits);

    static mpz_class make_from_bytes(quint8 *bytes, int length);
    static quint32 make_from_bytes_reverse(quint8* bytes);

    static void block_swap(quint32* a, quint32* b);

//    static mpz_class str_to_mpz(char* text);
//    static char* mpz_to_str(const mpz_class& text);
    //encrypt and decrypt blocks only length of 128
    ByteArray encrypt_qstr_128(const QString& text);
    QString decrypt_qstr_128(const ByteArray& text);

public:
    Twofish(const mpz_class& key, int key_length);

    mpz_class encrypt_mpz(const mpz_class& text);
    mpz_class decrypt_mpz(const mpz_class& text);

    mpz_class encrypt_str(const char* text);
    char* decrypt_str(const mpz_class& text);

    static mpz_class generateKey();

    ByteArray encrypt_qstr(const QString& text);
    QString decrypt_qstr(const ByteArray& text);
};

#endif // TWOFISH_H
