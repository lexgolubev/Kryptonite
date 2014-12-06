#ifndef CLIENT_H
#define CLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include "rsa/rsakey.h"
#include "server.h"

class Server;

class Client
{
private:
    QString name;
    RsaKey publicKey;
    RsaKey privateKey;

    QTcpSocket serverSocket;
    Server* server;

private:
    void getAllClients();
    void connectToPeer(QString name);

public:
    Client(QString name, RsaKey publicKey, RsaKey privateKey, int localPort, QString serverIp, int serverPort);

    void sendInitRequest(RsaKey publicKey, QString name);

    RsaKey getPublicKey();
    QString getName();

    ~Client();
};

#endif // CLIENT_H
