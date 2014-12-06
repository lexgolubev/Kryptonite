#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QHostAddress>

#include "rsa/rsakey.h"

class ClientInfo
{
private:
    QHostAddress hostAddress;
    RsaKey publicKey;
public:
    ClientInfo();
    ClientInfo(const QHostAddress& hostAddress, const RsaKey& publicKey);
    RsaKey& getPublicKey();
    QHostAddress& getHostAddress();
    ~ClientInfo();
};

#endif // CLIENTINFO_H
