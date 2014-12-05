#include <QtNetwork>

#include "connection.h"
#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    listen(QHostAddress::Any, PORT);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Connection *connection = new Connection(this, this);
    connection->setSocketDescriptor(socketDescriptor);
    emit newConnection(connection);
}

bool Server::addClient(QString name, const ClientInfo& info) {
    if (activeClients.contains(name)) {
        return false;
    }
    activeClients.insert(name, info);
    return true;
}

int Server::size() {
    return activeClients.size();
}

QList<QString> Server::clients() {
    return activeClients.keys();
}

ClientInfo Server::getPeerByName(QString name) {
    return activeClients.take(name);
}

Server::~Server() {
    activeClients.clear();
}
