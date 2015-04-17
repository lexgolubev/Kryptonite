#include "client.h"

Client::Client(QString name, int localPort, QString serverIp, int serverPort) {
    RsaKeyGenerator gen;
    gen.generate(1024);

    this->publicKey = gen.get_public_key();
    this->privateKey = gen.get_private_key();
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

void Client::connectToPeer(QString peerName) {
    QDataStream stream(serverSocket);

    QString request = "REQUEST_GET_PEER_BY_NAME";
    stream << request;

    stream << peerName;

    serverSocket->waitForReadyRead(5000);
    QString address;
    stream >> address;

    int port;
    stream >> port;

    RsaKey key;
    stream >> key;

    Connection* newConnection = new Connection(0);
    newConnection->setPeer(peerName);
    newConnection->setFriendRsaKey(key);
    newConnection->setOwnPrivateKey(privateKey);
    newConnection->setOwnPublicKey(publicKey);
    newConnection->setName(name);
    newConnection->connectToPeer(address, port);

    QThread* thread = new QThread();
    newConnection->moveToThread(thread);
    thread->start();
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    addConnection(peerName, newConnection);
    onNewConnection(newConnection);
}

QList<QString> Client::getAllClients() {
    QDataStream stream(serverSocket);
    QString req = "REQUEST_GET_ALL_CLIENTS";
    stream << req;

    QList<QString> peers;
    serverSocket->waitForReadyRead(5000);
    quint32 count;
    stream >> count;
    for (unsigned int i = 0; i < count; i++) {
        QString name;
        stream >> name;
        peers.push_back(name);
    }
    return peers;
}

bool Client::sendMessage(QString destination, QString msg) {
    if (connections.find(destination) == connections.end()) {
        connectToPeer(destination);
    }
    return connections[destination]->sendMessage(msg);

}

void Client::onMessageRecivied(QString msg) {
    Connection* connection = (Connection*)QObject::sender();
    QString user = connection->getPeer();
    qDebug() << "new message from " << user << ":" << msg;
    qDebug() << "client.recieve user:" << user;
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
    qDebug() << "client" << connection->getPeer() << "disconnected";
    connections.remove(connection->getPeer());
    qDebug() << connections.keys();
}

Client::~Client() {
    serverSocket->close();
}
