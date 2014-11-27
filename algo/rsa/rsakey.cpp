#include "rsakey.h"

RsaKey::RsaKey() {

}

RsaKey::RsaKey(mpz_class exp, mpz_class module) : exp(exp), module(module) {
}

mpz_class RsaKey::get_exp() const {
    return exp;
}

mpz_class RsaKey::get_module() const {
    return module;
}

void RsaKey::set_exp(mpz_class& value) {
    exp = value;
}

void RsaKey::set_module(mpz_class& value) {
    module = value;
}

QDataStream& operator <<(QDataStream& out,const RsaKey &key) {
    const char* str = key.get_exp().get_str(16).c_str();
    int length = mpz_sizeinbase(key.get_exp().get_mpz_t(), 16);
    out.writeRawData(str, length);

    str = key.get_module().get_str(16).c_str();
    length = mpz_sizeinbase(key.get_module().get_mpz_t(), 16);
    return out;
}

QDataStream& operator >>(QDataStream& in, RsaKey& key) {
    char tmp[512];

    in.readRawData(tmp, 512);
    mpz_class exp;
    exp.set_str(tmp, 16);
    key.set_exp(exp);

    in.readRawData(tmp, 512);
    mpz_class module;
    module.set_str(tmp, 16);
    key.set_module(module);

    return in;
}
