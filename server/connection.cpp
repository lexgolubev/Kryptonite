#include "connection.h"

Connection::Connection(QObject *parent, Server* server)
    : QTcpSocket(parent)
{
    this->server = server;
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

void Connection::processReadyRead()
{
    qDebug() << "new data";
    QDataStream stream(this);
    do {
        QString request;
        stream >> request;
        if (request == "REQUEST_CONNECT_SERVER") {
            onRequestConnect();
        } else if (request == "REQUEST_GET_ALL_CLIENTS") {
            onRequestGetAllClients();
        } else if (request == "REQUEST_GET_PEER_BY_NAME") {
            onRequestGetPeerByName();
        }
    } while (bytesAvailable() > 0);
}

void Connection::onRequestConnect() {
    QDataStream stream(this);

    QString name;
    stream >> name;
    this->name = name;

    RsaKey key;
    stream >> key;

    int port;
    stream >> port;

    QHostAddress address = peerAddress();

    ClientInfo info(address, port, key);

    if (server->addClient(name, info)) {
        qDebug() << "client" << name << "connected";
    } else {
        qDebug() << "This name already used. Please, select another name";
    }

    QString answer = "CONNECT_OK";
    stream << answer;
}

void Connection::onRequestGetAllClients() {
    qDebug() << "getAllClients";
    QDataStream stream(this);
    stream << (quint32)server->size() - 1;
    foreach (QString name, server->clients()) {
        if (this->name != name) {
            stream << name;
        }
    }
    waitForBytesWritten();
}

void Connection::onRequestGetPeerByName() {
    qDebug() << "getPeerByName";
    QDataStream stream(this);

    QString clientName;
    stream >> clientName;

    ClientInfo info = server->getPeerByName(clientName);
    stream << info.getHostAddress().toString();
    stream << info.getPort();
    stream << info.getPublicKey();
    waitForBytesWritten();
}

QString Connection::getName() {
    return name;
}
