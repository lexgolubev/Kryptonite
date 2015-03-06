#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "client.h"
#include "connection.h"

class Client;
//class Connection;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent, Client* client, int port);
    ~Server();
signals:
    void newConnection(Connection *connection);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    Client* client;
};


#endif // SERVER_H
