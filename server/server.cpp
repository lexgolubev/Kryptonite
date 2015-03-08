#include "server.h"

Server::Server(int port, QObject *parent) : QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Connection *connection = new Connection(this, this);
    connection->setSocketDescriptor(socketDescriptor);
    connect(connection, SIGNAL(disconnected()), this, SLOT(onConnectionDisconnected()));
    emit newConnection(connection);
}

bool Server::addClient(QString name, const ClientInfo& info) {
    if (activeClients.contains(name)) {
        qDebug() << "contains" << name;
        return false;
    }
//    activeClients.insert(name, info);
    activeClients[name] = info;
    qDebug() << "clients" << activeClients.keys();
    return true;
}

int Server::size() {
    return activeClients.size();
}

QList<QString> Server::clients() {
    return activeClients.keys();
}

ClientInfo Server::getPeerByName(QString name) {
    return activeClients[name];
}

void Server::onConnectionDisconnected() {
    Connection* connection = (Connection*) sender();
    qDebug() << "client" << connection->getName() << "disconnected";
    activeClients.remove(connection->getName());
    qDebug() << activeClients.keys();
}

Server::~Server() {
    activeClients.clear();
    close();
}
