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
    QString name;
public:
    ClientInfo();
    ClientInfo(const QHostAddress& hostAddress, int port, const RsaKey& publicKey, QString name);

    void setHostAddress(const QHostAddress& hostAddress);
    void setPort(int port);
    void setPublicKey(const RsaKey& key);
    void setName(QString name);

    QHostAddress getHostAddress() const;
    int getPort() const;
    RsaKey getPublicKey() const;
    QString getName() const;

    ~ClientInfo();
    friend QDataStream& operator >>(QDataStream& in, ClientInfo& info);
    friend QDataStream& operator <<(QDataStream& out,const ClientInfo& info);
};

//QDataStream& operator <<(QDataStream& out,const ClientInfo& info);
//QDataStream& operator >>(QDataStream& in, ClientInfo& info);

#endif // CLIENTINFO_H
