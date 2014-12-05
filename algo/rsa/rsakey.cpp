#include "rsakey.h"
#include <QDebug>

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
    QString exp = key.get_exp().get_str(16).c_str();
    out << exp;
    QString module = key.get_module().get_str(16).c_str();
    out << module;

    return out;
}

QDataStream& operator >>(QDataStream& in, RsaKey& key) {
    QString exp_str;
    QString module_str;
    in >> exp_str >> module_str;

    mpz_class exp;
    exp.set_str(exp_str.toStdString().c_str(), 16);
    key.set_exp(exp);

    mpz_class module;
    module.set_str(module_str.toStdString().c_str(), 16);
    key.set_module(module);

    return in;
}
