#ifndef CONNECTION_H
#define CONNECTION_H

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include "client.h"

class Client;

class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    Connection(QObject *parent, Client* client, bool connected = true, QString address = "",
               int port = 0);
signals:

protected:

private slots:
    void processReadyRead();

private:
    void req_init();
    void req_msg();
    bool send_msg(QString msg);
    void req_get_peer_by_name();
    void init_friend(QString address, int port);

    Client* client;
};

#endif
