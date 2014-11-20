#include "rsakey.h"

RsaKey::RsaKey() {

}

RsaKey::RsaKey(mpz_class exp, mpz_class module) : exp(exp), module(module) {
}

mpz_class RsaKey::get_exp() {
    return exp;
}

mpz_class RsaKey::get_module() {
    return module;
}
