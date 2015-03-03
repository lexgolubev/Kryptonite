#ifndef SERVERCONNECTIONTHREAD_H
#define SERVERCONNECTIONTHREAD_H

#include <QThread>
#include <QString>
#include "client.h"

class ServerConnectionThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        qDebug() << "started";
        client = new Client(name, publicKey, privateKey, localPort, serverIp, serverPort);
        client->connectToServer();
        connect(client, SIGNAL(recieveMessage(QString, QString)), this, SIGNAL(recieveMessage(QString,QString)));
        foreach (QString user, client->getAllClients()) {
            emit addUser(user);
            client->connectToPeer(user);
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
    void recieveMessage(QString user, QString message);

private slots:
    void onMessageRecivied(QString message) {
        emit recieveMessage(name, message);
    }

public slots:
    void onSendMessage(QString user, QString message) {
        client->sendMessage(user, message);
    }

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
