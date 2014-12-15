#ifndef CONNECTION_H
#define CONNECTION_H

#include <QHostAddress>
#include <QTcpSocket>
#include "rsa/rsakey.h"
#include "server.h"

class Server;

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    Connection(QObject *parent, Server* server);

    QString getName();

private slots:
    void processReadyRead();

private:
    void onRequestConnect();
    void onRequestGetAllClients();
    void onRequestGetPeerByName();

    Server* server;
    QString name;
};

#endif
