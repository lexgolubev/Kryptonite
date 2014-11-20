#include "rsakeygenerator.h"
#include <boost/random/random_device.hpp>
#include <QDebug>

RsaKeyGenerator::RsaKeyGenerator() : rand(gmp_randinit_default) {
    boost::random_device rand_device;
    rand.seed(rand_device());
}

mpz_class RsaKeyGenerator::next_prime(mpz_class bit_length)
{
    mpz_class tmp = rand.get_z_bits(bit_length);
    mpz_t tmp_res;
    mpz_init(tmp_res);
    mpz_nextprime(tmp_res, tmp.get_mpz_t());
    mpz_class res = mpz_class(tmp_res);
    mpz_clear(tmp_res);
    return res;
}

void RsaKeyGenerator::generate(mp_bitcnt_t key_length) {
    mpz_class bit_length = key_length / 2;

    mpz_class p = next_prime(bit_length);
    mpz_class q = next_prime(bit_length);

    //n is module
    mpz_class n = p * q;
    //phi_n is eulier_function(n)
    mpz_class phi_n = (p - 1) * (q - 1);
    //select prime number e, e is public key
    mpz_t gcd;
    mpz_init(gcd);
    mpz_t tmp_e;
    mpz_init(tmp_e);
    mpz_class e = 65536;
    do {
        mpz_nextprime(tmp_e, e.get_mpz_t());
        e = mpz_class(tmp_e);
        mpz_gcd(gcd, e.get_mpz_t(), phi_n.get_mpz_t());
    } while(mpz_cmp_si(gcd, 1));
    //find d, d is private key
    mpz_t tmp_d;
    mpz_init(tmp_d);
    mpz_invert(tmp_d, e.get_mpz_t(), phi_n.get_mpz_t());
    mpz_class d = mpz_class(tmp_d);

    mpz_clear(gcd);
    mpz_clear(tmp_e);
    mpz_clear(tmp_d);

    private_key = RsaKey(d, n);
    public_key = RsaKey(e, n);
}

RsaKey RsaKeyGenerator::get_private_key() {
    return private_key;
}

RsaKey RsaKeyGenerator::get_public_key() {
    return public_key;
}
