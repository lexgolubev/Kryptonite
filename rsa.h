#ifndef RSA_H
#define RSA_H

#include <gmpxx.h>
#include "rsakey.h"

class Rsa
{
private:
    Rsa();
public:
    static mpz_class crypt(mpz_class msg, RsaKey key);
};

#endif // RSA_H
