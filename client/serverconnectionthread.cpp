#include "serverconnectionthread.h"

ServerConnectionThread::ServerConnectionThread(QString name, int localPort, QString serverIp, int serverPort)
{
//    client = new Client(name, publicKey, privateKey, localPort, serverIp, serverPort);
    this->name = name;
    this->publicKey = publicKey;
    this->privateKey = privateKey;
    this->localPort = localPort;
    this->serverIp = serverIp;
    this->serverPort = serverPort;
}

ServerConnectionThread::~ServerConnectionThread()
{
}

