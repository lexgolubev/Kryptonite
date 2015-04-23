#include "dht.h"

Dht::Dht() : bootstrapSocket(nullptr)
{
}

void Dht::setBootstrapServer(QString ip, int port) {
    if (bootstrapSocket != nullptr) {
        bootstrapSocket->close();
    }
    bootstrapSocket = new QTcpSocket();
    bootstrapSocket->connectToHost(ip, port);
}

QList<QPair<QString, int>> Dht::getListOfPosibleUsers(int size) {
    QDataStream stream(bootstrapSocket);
    QString request = "REQUEST_GET_PEERS";
    stream << request;
    stream << size;
    bootstrapSocket->waitForReadyRead();

    QList<QPair<QString, int>> result;
    QString end = "END";
    QString ip;
    int port;
    stream >> ip;
    while (ip != end) {
        stream >> port;
        result.push_back(QPair<QString, int>(ip, port));
        stream >> ip;
    }
    return result;
}

Dht::~Dht()
{

}

