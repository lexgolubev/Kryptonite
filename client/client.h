#ifndef CLIENT_H
#define CLIENT_H

#include <QtGlobal>
#include <QHostAddress>
#include <QVector>
#include <QTcpSocket>
#include "../algo/rsa/rsakey.h"
#include "server.h"

class Server;

class Client
{
private:
    const QString SERVER_IP = "192.168.0.104";
    const int SERVER_PORT = 8090;
    const int CLIENT_PORT = 8091;
private:
    QString name;
    RsaKey public_key;
    RsaKey private_key;

    QTcpSocket server_socket;
    Server* server;
private:
    void getAllClients();
    void connectToPeer(QString name);
public:
    Client(QString name, RsaKey public_key, RsaKey private_key, int port);
    RsaKey getPublicKey();
    QString getName();
    ~Client();
    void send_init_req(RsaKey public_key, QString name);
};

#endif // CLIENT_H
