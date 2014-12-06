#include "server.h"

Server::Server(QObject *parent, Client* client, int port)
    : QTcpServer(parent)
{
    qDebug() << "listen" << port;
    listen(QHostAddress::Any, port);
    this->client = client;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Connection *connection = new Connection(this, client);
    connection->setSocketDescriptor(socketDescriptor);
    emit newConnection(connection);
}

Server::~Server() {
    close();
}
