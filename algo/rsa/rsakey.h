#ifndef RSAKEY_H
#define RSAKEY_H

#include <gmpxx.h>

class RsaKey
{
    friend class RsaKeyGenerator;
private:
    mpz_class exp;
    mpz_class module;
public:
    RsaKey();
    RsaKey(mpz_class exp, mpz_class module);

    mpz_class get_exp();
    mpz_class get_module();
};

#endif // RSAKEY_H
