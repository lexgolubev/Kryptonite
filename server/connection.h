#ifndef CONNECTION_H
#define CONNECTION_H

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include "server.h"

static const int MaxBufferSize = 1024000;

class Server;

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    Connection(QObject *parent, Server* server);
signals:

protected:

private slots:
    void processReadyRead();

private:
    void req_init();
    void req_get_all_clients();
    void req_get_peer_by_name();

    Server* server;
    QString name;
};

#endif
