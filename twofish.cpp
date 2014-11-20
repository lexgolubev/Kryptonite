#include "twofish.h"
#include <QDebug>

Twofish::Twofish(const mpz_class& key, int key_length) {
    key_shedule(key, key_length);
}

void Twofish::key_shedule(const mpz_class &key, int key_length) {
    k = (key_length + 63) / 64;
    mpz_class extended_key = key << (k * 64 - key_length);
    key_length = k * 64;
    quint32* m = split_mpz_32_little_endian(extended_key, key_length);
    m_e = new quint32[k];
    m_o = new quint32[k];
    for (int i = 0; i < k; i++) {
        m_e[i] = m[2 * i + 1];
        m_o[i] = m[2 * i];
    }
    quint8* bytes = split_mpz_8(key, key_length);
    for (int i = 0; i < k; i++) {
        quint8* tmp_s = multiply_gf(RS_MATRIX, bytes + 8 * i, RS_VECTOR);
        s[k - i - 1] = make_from_bytes_reverse(tmp_s);
        delete tmp_s;
    }
    quint32 p = (1 << 24) + (1 << 16) + (1 << 8) + 1; // 2 ^ 24 + 2 ^ 16 + 2 ^ 8 + 2 ^ 0
    for (int i = 0; i < 20; i++) {
        quint64 a = h(2 * i * p, m_o, k);
        quint64 b = rol(h((2 * i + 1) * p, m_e, k), 8);
        keys[2 * i] = (quint32)(a + b);
        keys[2 * i + 1] = rol((quint32)(a + 2 * b),9);
    }
    delete m;
}

mpz_class Twofish::encrypt(const mpz_class &text) {
    quint32* words = split_mpz_32_little_endian(text, 128);
    //whitening
    for (int i = 0; i < 4; i++) {
        words[i] ^= keys[i];
    }
    //fiestel
    for (int i = 0; i < 16; i++) {
        std::pair<quint32, quint32> p = f(words[0], words[1], i);
        quint32 tmp0 = words[0];
        quint32 tmp1 = words[1];
        words[0] = ror(words[2] ^ p.first, 1);
        words[1] = rol(words[3], 1) ^ p.second;
        words[2] = tmp0;
        words[3] = tmp1;
    }
//    for (int i = 0; i < 16; i++) {
//        int i1 = (i % 2) * 2;
//        int i2 = ((i + 1) % 2) * 2;
//        std::pair<quint32, quint32> p = f(words[i1], words[i1 + 1], i);
//        words[i2] = ror(words[i2] ^ p.first, 1);
//        words[i2 + 1] = rol(words[i2 + 1], 1) ^ p.second;
//    }
    quint32 encrypted[4];
    for (int i = 0; i < 4; i++) {
        encrypted[i] = words[(i + 2) % 4] ^ keys[i + 4];
    }
//    quint32 encrypted[4];
    //    for (int i = 0; i < 4; i++) {
    //        encrypted[i] ^= keys[i + 4];
    //    }
    quint8 c[16];
    for (int i = 0; i < 16; i++) {
        c[i] = (encrypted[i / 4] / (1 << (8 * (i % 4)))) % (1 << 8);
    }
//    quint8 c[16];
//    for (int i = 0; i < 16; i++) {
//        c[i] = (words[i / 4] / (1 << (8 * (i % 4)))) % (1 << 8);
//    }

    return make_from_bytes(c, 16);
}

mpz_class Twofish::decrypt(const mpz_class &text) {
    quint32* words = split_mpz_32_little_endian(text, 128);
    for (int i = 0; i < 4; i++) {
        words[i] ^= keys[i + 4];
    }
    block_swap(words, words + 2);
    block_swap(words + 1, words + 3);
    for (int i = 15; i >= 0; i--) {
        std::pair<quint32, quint32> p = f(words[2], words[3], i);
        quint32 tmp0 = words[2];
        quint32 tmp1 = words[3];
        words[2] = rol(words[0], 1) ^ p.first;
        words[3] = ror(words[1] ^ p.second, 1);
        words[0] = tmp0;
        words[1] = tmp1;
    }
    quint32 encrypted[4];
    for (int i = 0; i < 4; i++) {
        encrypted[i] = words[i] ^ keys[i];
    }
    quint8 c[16];
    for (int i = 0; i < 16; i++) {
        c[i] = (encrypted[i / 4] / (1 << (8 * (i % 4)))) % (1 << 8);
    }
    return make_from_bytes(c, 16);
}

void Twofish::block_swap(quint32* a, quint32* b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

std::pair<quint32, quint32> Twofish::f(quint32 r0, quint32 r1, int round) {
    quint64 t0 = g(r0);
    quint64 t1 = g(rol(r1, 8));
    quint32 f0 = (quint32)(t0 + t1 + keys[2 * round + 8]);
    quint32 f1 = (quint32)(t0 + 2 * t1 + keys[2 * round + 9]);
    return std::pair<quint32, quint32>(f0, f1);
}

quint32 Twofish::g(quint32 word) {
    return h(word, s, k);
}

quint32 Twofish::h(quint32 x, quint32* l, int k) {
    quint8* y = split_32_8_reverse(x);
    quint8* lbytes;
    switch (k) {
    case 4:
        lbytes = split_32_8_reverse(l[3]);
        y[0] = q1(y[0]) ^ lbytes[0];
        y[1] = q0(y[1]) ^ lbytes[1];
        y[2] = q0(y[2]) ^ lbytes[2];
        y[3] = q1(y[3]) ^ lbytes[3];
        delete lbytes;
        k--;
    case 3:
        lbytes = split_32_8_reverse(l[2]);
        y[0] = q1(y[0]) ^ lbytes[0];
        y[1] = q1(y[1]) ^ lbytes[1];
        y[2] = q0(y[2]) ^ lbytes[2];
        y[3] = q0(y[3]) ^ lbytes[3];
        delete lbytes;
        k--;
    case 2:
        quint8* lbytes1 = split_32_8_reverse(l[1]);
        quint8* lbytes0 = split_32_8_reverse(l[0]);
        y[0] = q1(q0(q0(y[0]) ^ lbytes1[0]) ^ lbytes0[0]);
        y[1] = q0(q0(q1(y[1]) ^ lbytes1[1]) ^ lbytes0[1]);
        y[2] = q1(q1(q0(y[2]) ^ lbytes1[2]) ^ lbytes0[2]);
        y[3] = q0(q1(q1(y[3]) ^ lbytes1[3]) ^ lbytes0[3]);
        delete lbytes1;
        delete lbytes0;
    }
    quint8* tmp = multiply_gf(MDS_MATRIX, y, MDS_VECTOR);
    quint32 result = make_from_bytes_reverse(tmp);
    delete tmp;
    delete y;
    return result;
}

quint8 Twofish::q0_q1_permutation(quint8 word, bool q) {
    auto& t = q ? Q0 : Q1;
    quint8 a0 = word / 16;
    quint8 b0 = word % 16;
    quint8 a1 = a0 ^ b0;
    quint8 b1 = a0 ^ ror4(b0, 1) ^ (8 * a0 % 16);
    quint8 a2 = t[0][a1];
    quint8 b2 = t[1][b1];
    quint8 a3 = a2 ^ b2;
    quint8 b3 = a2 ^ ror4(b2, 1) ^ (8 * a2 % 16);
    quint8 a4 = t[2][a3];
    quint8 b4 = t[3][b3];
    return 16 * b4 + a4;
}

quint8 Twofish::q0(quint8 word) {
    return q0_q1_permutation(word, true);
}

quint8 Twofish::q1(quint8 word) {
    return q0_q1_permutation(word, false);
}

quint16 Twofish::multiply_gf(quint8 first, quint8 second) {
    quint16 res = 0;
    for (int i = 0; i < 8; i++) {
        quint8 bit = (second / (1 << i)) % 2;
        res ^= (first * bit) << i;
    }
    return res;
}

quint8 Twofish::mod_gf(quint16 first, quint16 second) {
    mpz_class op1 = first;
    mpz_class op2 = second;
    size_t op1_bit_count = mpz_sizeinbase(op1.get_mpz_t(), 2);
    size_t op2_bit_count = mpz_sizeinbase(op2.get_mpz_t(), 2);
    while (op1_bit_count >= op2_bit_count) {
        op1 ^= op2 << (op1_bit_count - op2_bit_count);
        op1_bit_count = mpz_sizeinbase(op1.get_mpz_t(), 2);
    };
    return (quint8) op1.get_ui();
}

template<size_t N, size_t M>
quint8* Twofish::multiply_gf(const quint8 (&matrix)[N][M], const quint8* vector, quint16 base) {
    quint8* res = new quint8[N];
    memset(res, 0, sizeof(quint8) * N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            res[i] ^= mod_gf(multiply_gf(matrix[i][j], vector[j]), base);
        }
    }
    return res;
}


quint8* Twofish::split_32_8_reverse(quint32 word) {
    int k = 4;
    quint8* result = new quint8[k];
    for (int i = 0; i < k; i++) {
        result[i] = word / (1 << 8 * i) % 0x100;
    }
    return result;
}

quint32* Twofish::split_mpz_32_little_endian(const mpz_class& word, int key_length) {
    quint8* bytes = split_mpz_8(word, key_length);
    int k = key_length / 32;
    quint32* result = new quint32[k];
    memset(result, 0, k * sizeof(quint32));
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < 4; j++) {
            result[i] += (quint32)bytes[4 * i + j] << (8 * j);
        }
    }
    delete bytes;
    return result;
}

quint8* Twofish::split_mpz_8(const mpz_class& word, int key_length) {
    int k = key_length / 8;
    quint8* result = new quint8[k];
    for (int i = 0; i < k; i++) {
        mpz_class tmp = word / ((mpz_class)1 << (8 * (k - i - 1)));
        tmp %= 0x100;
        result[i] = (quint8)tmp.get_ui();
    }
    return result;
}

quint32 Twofish::make_from_bytes_reverse(quint8* bytes) {
    int k = 4;
    quint32 result = 0;
    for (int i = 0; i < k; i++) {
        result += ((quint32) bytes[i] << 8 * i);
    }
    return result;
}

mpz_class Twofish::make_from_bytes(quint8 *bytes, int length) {
    mpz_class result = 0;
    for (int i = 0; i < length; i++) {
        result <<= 8;
        result += (mpz_class)(bytes[i]);
    }
    return result;
}

quint8 Twofish::ror4(quint8 word, int bits) {
    quint8 right_bits = word % (1 << bits);
    right_bits <<= 4 - bits;
    quint8 left_bits = word >> bits;
    return right_bits + left_bits;
}

quint32 Twofish::ror(quint32 word, int bits) {
    quint32 right_bits = word << (32 - bits);
    quint32 left_bits = word >> bits;
    return right_bits + left_bits;
}

quint32 Twofish::rol(quint32 word, int bits) {
    quint32 right_bits = word << bits;
    quint32 left_bits = word >> (32 - bits);
    return right_bits + left_bits;
}
