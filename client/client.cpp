#include "client.h"
#include <iostream>

Client::Client(QString name, RsaKey publicKey, RsaKey privateKey, int localPort, QString serverIp, int serverPort) {
    this->publicKey = publicKey;
    this->privateKey = privateKey;
    this->name = name;
    this->localPort = localPort;
    this->serverIp = serverIp;
    this->serverPort = serverPort;
    serverSocket = new QTcpSocket();
    server = new Server(0, this, localPort);
    QThread* thread = new QThread(this);
    thread->start();
    connect(server, SIGNAL(newConnection(Connection*)), this, SLOT(onNewConnection(Connection*)));
}

bool Client::connectToServer() {
    QHostAddress serverAddress(serverIp);
    serverSocket->connectToHost(serverAddress, serverPort);
    if (serverSocket->waitForConnected()) {
        qDebug() << "connected successfully";
        return connectToServer(publicKey, name);
    } else {
        qDebug() << "connection fail";
    }
    return false;
}

bool Client::connectToServer(RsaKey publicKey, QString name)
{
    QDataStream stream(serverSocket);
    QString request = "REQUEST_CONNECT_SERVER";
    stream << request;
    stream << name;
    stream << publicKey;
    stream << localPort;
    serverSocket->waitForReadyRead();
    QString answer;
    stream >> answer;
    return answer == "CONNECT_OK";
}

void Client::connectToPeer(QString name) {
    QDataStream stream(serverSocket);

    QString request = "REQUEST_GET_PEER_BY_NAME";
    stream << request;

    stream << name;

    serverSocket->waitForReadyRead(5000);
    QString address;
    stream >> address;

    int port;
    stream >> port;

    RsaKey key;
    stream >> key;

//    qDebug() << "data recivied from server";

//    qDebug() << "another user info:";
//    qDebug() << "\taddress: " << address;
//    qDebug() << "\tport: " << port;
//    qDebug() << "\tkey: " << "exp = " << key.get_exp().get_str(16).c_str() << ", mod = " << key.get_module().get_str(16).c_str();

    Connection* newConnection = new Connection(0, this, false, address, port);
    QThread* thread = new QThread();
    thread->start();
    newConnection->moveToThread(thread);
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connections[name] = newConnection;
    connect(newConnection, SIGNAL(messageRecivied(QString)), this, SLOT(onMessageRecivied(QString)));
}

QList<QString> Client::getAllClients() {
    QDataStream stream(serverSocket);
    QString req = "REQUEST_GET_ALL_CLIENTS";
    stream << req;

    QList<QString> peers;
    serverSocket->waitForReadyRead(5000);
    quint32 count;
    stream >> count;
    for (int i = 0; i < count; i++) {
        QString name;
        stream >> name;
        peers.push_back(name);
    }
    return peers;
}

bool Client::sendMessage(QString destination, QString msg) {
    return connections[destination]->sendMessage(msg);
}

void Client::onMessageRecivied(QString msg) {
    Connection* connection = (Connection*)QObject::sender();
    QString user = connection->getPeerName();
    std::cout << "new message from " << user.toStdString() << ":" << msg.toStdString();
    emit recieveMessage(user, msg);
}

void Client::onNewConnection(Connection *newConnection) {
    connect(newConnection, SIGNAL(messageRecivied(QString)), this, SLOT(onMessageRecivied(QString)));
}

RsaKey Client::getPublicKey() {
    return publicKey;
}

RsaKey Client::getPrivateKey() {
    return privateKey;
}

QString Client::getName() {
    return name;
}

void Client::addConnection(QString name, Connection* newconnection) {
    connections[name] = newconnection;
}

void Client::onConnectionDisconnected() {
    Connection* connection = (Connection*) sender();
    qDebug() << "client" << connection->getPeerName() << "disconnected";
    connections.remove(connection->getPeerName());
    qDebug() << connections.keys();
}

Client::~Client() {
    serverSocket->close();
}
