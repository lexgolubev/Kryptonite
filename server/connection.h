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

private slots:
    void processReadyRead();

private:
    void requestInit();
    void requestGetAllClients();
    void requestGetPeerByName();

    Server* server;
    QString name;
};

#endif
