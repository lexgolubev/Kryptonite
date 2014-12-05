#include "connection.h"

#include <QtNetwork>
#include "rsa/rsakey.h"

Connection::Connection(QObject *parent, Server* server)
    : QTcpSocket(parent)
{
    this->server = server;

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

void Connection::processReadyRead()
{
    qDebug() << "ready";
    QDataStream stream(this);
    do {
        QString request;
        stream >> request;
        if (request == "REQ_INIT") {
            req_init();
        } else if (request == "REQ_GET_ALL_CLIENTS") {
            req_get_all_clients();
        } else if (request == "REQ_GET_PEER_BY_NAME") {
            req_get_peer_by_name();
        }
    } while (bytesAvailable() > 0);
}

void Connection::req_init() {
    QDataStream stream(this);

    QHostAddress address = peerAddress();

    QString name;
    stream >> name;
    this->name = name;
    qDebug() << name;

    RsaKey key;
    stream >> key;
    qDebug() << "exp = " << key.get_exp().get_str(16).c_str() << ", mod = " << key.get_module().get_str(16).c_str();

    ClientInfo info(address, key);

    if (!server->addClient(name, info)) {
        qDebug() << "This name already used. Please, select another name";
    }

    QString answer = "OK";
    stream << answer;
}

void Connection::req_get_all_clients() {
    QDataStream stream(this);
    stream << (quint32)server->size() - 1;
    foreach (QString name, server->clients()) {
        if (this->name != name) {
            stream << name;
        }
    }
}

void Connection::req_get_peer_by_name() {
    QDataStream stream(this);
    QString clientName;
    stream >> clientName;
    ClientInfo info = server->getPeerByName(clientName);
    stream << info.getHostAddress().toString();
    stream << info.getPublicKey();
}
