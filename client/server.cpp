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
    Connection *connection = new Connection(0);
    connection->setSocketDescriptor(socketDescriptor);
    connection->setOwnPrivateKey(client->getPrivateKey());
    connection->setOwnPublicKey(client->getPublicKey());
    connection->setName(client->getName());
    emit newConnection(connection);
}

Server::~Server() {
    close();
}
