#ifndef RSAKEYGENERATOR_H
#define RSAKEYGENERATOR_H

#include <gmpxx.h>
#include "rsakey.h"

class RsaKeyGenerator
{
private:
    gmp_randclass rand;

    RsaKey private_key;
    RsaKey public_key;
public:
    RsaKeyGenerator();

    void generate(mp_bitcnt_t key_length);

    RsaKey get_private_key();
    RsaKey get_public_key();
    mpz_class next_prime(mpz_class bit_length);
};

#endif // RSAKEYGENERATOR_H
