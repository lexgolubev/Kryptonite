#ifndef CLIENT_H
#define CLIENT_H

#include <QtGlobal>
#include <QHostAddress>
#include <QVector>
#include <QTcpSocket>
#include "../algo/rsa/rsakey.h"

class Client
{
private:
    const QString SERVER_IP = "192.168.0.4";
private:
    RsaKey public_key;
    RsaKey private_key;

    QTcpSocket server_socket;
public:
    Client(QString name, RsaKey public_key, RsaKey private_key);
    ~Client();
};

#endif // CLIENT_H
