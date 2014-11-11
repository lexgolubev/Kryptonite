#include "mainwindow.h"
#include <QApplication>
#include "iostream"

#include "rsakeygenerator.h"
#include <gmp.h>
#include "rsa.h"
#include "rsakey.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    mp_bitcnt_t key_length = 128;
    RsaKeyGenerator gen;
    gen.generate(key_length);

    mpz_class n = gen.get_private_key().get_module();
    mpz_class e = gen.get_public_key().get_exp();
    mpz_class d = gen.get_private_key().get_exp();

    std::cout << "n = " << n.get_str() << std::endl;
    std::cout << "e = " << e.get_str() << std::endl;
    std::cout << "d = " << d.get_str() << std::endl;

    mpz_class msg = 256759;
    std::cout << "msg = " << msg.get_str() << std::endl;

    mpz_class encrypted = Rsa::crypt(msg, gen.get_public_key());
    std::cout << "encrypted = " << encrypted.get_str() << std::endl;

    mpz_class decrypted = Rsa::crypt(encrypted, gen.get_private_key());
    std::cout << "decrypted = " << decrypted.get_str(10) << std::endl;

    return a.exec();
}
