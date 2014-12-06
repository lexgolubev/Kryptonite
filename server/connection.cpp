#include "connection.h"

Connection::Connection(QObject *parent, Server* server)
    : QTcpSocket(parent)
{
    this->server = server;
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

void Connection::processReadyRead()
{
    QDataStream stream(this);
    do {
        QString request;
        stream >> request;
        if (request == "REQ_INIT") {
            requestInit();
        } else if (request == "REQ_GET_ALL_CLIENTS") {
            requestGetAllClients();
        } else if (request == "REQ_GET_PEER_BY_NAME") {
            requestGetPeerByName();
        }
    } while (bytesAvailable() > 0);
}

void Connection::requestInit() {
    QDataStream stream(this);

    QString name;
    stream >> name;
    this->name = name;
    qDebug() << name;

    RsaKey key;
    stream >> key;
    qDebug() << "exp = " << key.get_exp().get_str(16).c_str() << ", mod = " << key.get_module().get_str(16).c_str();

    QHostAddress address = peerAddress();

    ClientInfo info(address, key);

    if (!server->addClient(name, info)) {
        qDebug() << "This name already used. Please, select another name";
    }

    QString answer = "OK";
    stream << answer;
}

void Connection::requestGetAllClients() {
    QDataStream stream(this);
    stream << (quint32)server->size() - 1;
    foreach (QString name, server->clients()) {
        if (this->name != name) {
            stream << name;
        }
    }
}

void Connection::requestGetPeerByName() {
    QDataStream stream(this);

    QString clientName;
    stream >> clientName;

    ClientInfo info = server->getPeerByName(clientName);
    stream << info.getHostAddress().toString();
    stream << info.getPublicKey();
}
