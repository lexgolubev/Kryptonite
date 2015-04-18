#include "dht.h"

Dht::Dht()
{
}

void setBootstrapServer(QString ip, int port) {
    QTcpSocket* serverSocket = new QTcpSocket();
    serverSocket->connectToHost(ip, port);
    QDataStream stream(serverSocket);
    serverSocket->waitForReadyRead();
}

Dht::~Dht()
{

}

