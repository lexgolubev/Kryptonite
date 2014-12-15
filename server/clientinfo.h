#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QHostAddress>

#include "rsa/rsakey.h"

class ClientInfo
{
private:
    QHostAddress hostAddress;
    RsaKey publicKey;
    int port;
public:
    ClientInfo();
    ClientInfo(const QHostAddress& hostAddress, int port, const RsaKey& publicKey);
    RsaKey& getPublicKey();
    QHostAddress& getHostAddress();
    int getPort();
    ~ClientInfo();
};

#endif // CLIENTINFO_H
