#ifndef RSAKEY_H
#define RSAKEY_H

#include <QtGlobal>
#include <QDataStream>
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

    mpz_class get_exp() const;
    mpz_class get_module() const;

    void set_exp(mpz_class& value);
    void set_module(mpz_class& value);
};

QDataStream& operator <<(QDataStream& out,const RsaKey &key);
QDataStream& operator >>(QDataStream& in, RsaKey& key);

#endif // RSAKEY_H
