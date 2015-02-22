#ifndef CLIENT_H
#define CLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include <QMap>
#include "rsa/rsakey.h"
#include "server.h"
#include "connection.h"

class Server;

class Connection;

class Client : public QObject
{
    Q_OBJECT
private:
    QString name;
    RsaKey publicKey;
    RsaKey privateKey;
    int localPort;
    QString serverIp;
    int serverPort;

    QTcpSocket serverSocket;
    Server* server;

    QMap<QString, Connection*> connections;

public:
    Client(QString name, RsaKey publicKey, RsaKey privateKey, int localPort, QString serverIp, int serverPort);

    bool connectToServer(RsaKey publicKey, QString name);
    void addConnection(QString name, Connection* newconnection);

    void connectToPeer(QString name);
    bool sendMessage(QString destination, QString msg);
    QList<QString> getAllClients();

    RsaKey getPublicKey();
    RsaKey getPrivateKey();
    QString getName();

    ~Client();
public slots:
    void onMessageRecivied(QString msg);
    void onNewConnection(Connection* newConnection);
    void onConnectionDisconnected();
    bool connectToServer();
};

#endif // CLIENT_H
