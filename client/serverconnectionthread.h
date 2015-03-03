#ifndef SERVERCONNECTIONTHREAD_H
#define SERVERCONNECTIONTHREAD_H

#include <QThread>
#include <QString>
#include "client.h"

class ServerConnectionThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        client = new Client(name, publicKey, privateKey, localPort, serverIp, serverPort);
        client->connectToServer();
        foreach (QString user, client->getAllClients()) {
            emit addUser(user);
            qDebug() << "user: " << user;
        }
    }
public:
    ServerConnectionThread(QString name, RsaKey publicKey, RsaKey privateKey,
                           int localPort, QString serverIp, int serverPort);
    ~ServerConnectionThread();

signals:
    void addUser(QString name);
    void removeUser(QString name);

private:
    Client* client;

    QString name;
    RsaKey publicKey;
    RsaKey privateKey;
    int localPort;
    QString serverIp;
    int serverPort;
};

#endif // SERVERCONNECTIONTHREAD_H
