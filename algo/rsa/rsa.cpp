#include "rsa.h"

Rsa::Rsa()
{
}

mpz_class Rsa::crypt(mpz_class msg, RsaKey key) {
    mpz_class exp = key.get_exp();
    mpz_class module = key.get_module();

    mpz_t res;
    mpz_init(res);

    mpz_powm(res, msg.get_mpz_t(), exp.get_mpz_t(), module.get_mpz_t());

    return mpz_class(res);
}
