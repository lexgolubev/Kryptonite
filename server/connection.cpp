#include "connection.h"

Connection::Connection(QObject *parent)
    : QTcpSocket(parent)
{
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

void Connection::processReadyRead()
{
    QDataStream stream(this);
    do {
        QString request;
        stream >> request;
        if (request == "REQUEST_CONNECT_SERVER") {
            emit requestConnect();
        } else if (request == "REQUEST_GET_ALL_CLIENTS") {
            emit requestGetAllClients();
        } else if (request == "REQUEST_GET_PEER_BY_NAME") {
            emit requestGetPeerByName();
        }
    } while (bytesAvailable() > 0);
}

QString Connection::getName() {
    return name;
}

void Connection::setName(QString name) {
    this->name = name;
}
