#ifndef CONNECTION_H
#define CONNECTION_H

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include "client.h"
#include "rsa/rsa.h"
#include "twofish/twofish.h"

class Client;

class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    Connection(QObject *parent, Client* client, bool connected = true, QString address = "",
               int port = 0);
    bool sendMessage(QString msg);
    QString getName();
    void setName(QString peerName);
signals:
    void messageRecivied(QString msg);

private slots:
    void processReadyRead();

private:
    bool connectToPeer(QString address, int port);
    bool onRequestConnect();
    void onMessageRecivied();
    void req_get_peer_by_name();
private:
    Client* client;
    QString peerName;
    mpz_class twofish_key;
    Twofish* twofish;
};

#endif
