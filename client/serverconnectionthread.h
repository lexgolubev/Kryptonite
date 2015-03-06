#ifndef SERVERCONNECTIONTHREAD_H
#define SERVERCONNECTIONTHREAD_H

#include <QThread>
#include <QString>
#include "client.h"
#include <QEventLoop>

class ServerConnectionThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        QEventLoop loop;
        client = new Client(name, publicKey, privateKey, localPort, serverIp, serverPort);
        client->connectToServer();
        connect(client, SIGNAL(recieveMessage(QString, QString)), this, SLOT(onMessageRecivied(QString,QString)));
        foreach (QString user, client->getAllClients()) {
            emit addUser(user);
            client->connectToPeer(user);
        }
        loop.exec();
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
    void onMessageRecivied(QString user, QString message) {
        qDebug() << "user rcv:" << user;
        emit recieveMessage(user, message);
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
