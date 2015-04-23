#include "server.h"

Server::Server(int port, QObject *parent) : QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Connection *connection = new Connection(this);
    connection->setSocketDescriptor(socketDescriptor);
    connect(connection, SIGNAL(disconnected()),         this, SLOT(onConnectionDisconnected()));
    connect(connection, SIGNAL(requestConnect()),       this, SLOT(onRequestConnect()));
    connect(connection, SIGNAL(requestGetAllClients()), this, SLOT(onRequestGetAllClients()));
    connect(connection, SIGNAL(requestGetPeerByName()), this, SLOT(onRequestGetPeerByName()));
    connect(connection, SIGNAL(requestGetPeers(int)),   this, SLOT(onRequestGetPeers(int)));
    emit connected(connection);
}

bool Server::addClient(QString name, const ClientInfo& info) {
    if (activeClients.contains(name)) {
        qDebug() << "contains" << name;
        return false;
    }
    activeClients[name] = info;
    qDebug() << "clients" << activeClients.keys();
    return true;
}

void Server::onConnectionDisconnected() {
    Connection* connection = (Connection*) sender();
    qDebug() << "client" << connection->getName() << "disconnected";
    activeClients.remove(connection->getName());
    qDebug() << activeClients.keys();
}

void Server::onRequestConnect() {
    Connection* connection = qobject_cast<Connection*>(sender());

    QDataStream stream(connection);

    QString name;
    stream >> name;
    connection->setName(name);

    RsaKey key;
    stream >> key;

    int port;
    stream >> port;

    QHostAddress address = connection->peerAddress();

    ClientInfo info(address, port, key);

    if (this->addClient(name, info)) {
        qDebug() << "client" << name << "connected";
    } else {
        qDebug() << "This name already used. Please, select another name";
    }

    QString answer = "CONNECT_OK";
    stream << answer;
}

void Server::onRequestGetAllClients() {
    qDebug() << "getAllClients";
    Connection* connection = qobject_cast<Connection*>(sender());
    QDataStream stream(connection);
    stream << (quint32)activeClients.size() - 1;
    foreach (QString name, activeClients.keys()) {
        if (connection->getName() != name) {
            stream << name;
        }
    }
    connection->waitForBytesWritten();
}

void Server::onRequestGetPeerByName() {
    qDebug() << "getPeerByName";
    Connection* connection = qobject_cast<Connection*>(sender());
    QDataStream stream(connection);

    QString clientName;
    stream >> clientName;

    ClientInfo info = activeClients[clientName];
    stream << info.getHostAddress().toString();
    stream << info.getPort();
    stream << info.getPublicKey();
    connection->waitForBytesWritten();
}

void Server::onRequestGetPeers(int size) {
    Connection* connection = qobject_cast<Connection*>(sender());
    QDataStream stream(connection);

    auto values = activeClients.values();
    if (values.size() < size) {
        foreach (ClientInfo info, values) {
            stream << info.getHostAddress().toString();
            stream << info.getPort();
        }
        QString end = "END";
        stream << end;
    } else {
        int random = qrand() % size;
        for (int i = 0; i < size; i++) {
            ClientInfo info = values[random];
            stream << info.getHostAddress().toString();
            stream << info.getPort();
        }
        QString end = "END";
        stream << end;
    }
    connection->waitForBytesWritten();
}

Server::~Server() {
    activeClients.clear();
    close();
}
