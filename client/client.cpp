#include "client.h"

Client::Client(QString name, RsaKey publicKey, RsaKey privateKey, int localPort, QString serverIp, int serverPort) {
    this->publicKey = publicKey;
    this->privateKey = privateKey;
    this->name = name;
    server = new Server(0, this, localPort);

    QHostAddress serverAddress(serverIp);
    serverSocket.connectToHost(serverAddress, serverPort);

    if (serverSocket.waitForConnected()) {
        qDebug() << "connected successfully";
        sendInitRequest(publicKey, name);
        getAllClients();
    } else {
        qDebug() << "connection fail";
    }
}

void Client::sendInitRequest(RsaKey publicKey, QString name)
{
    QDataStream stream(&serverSocket);
    QString request = "REQ_INIT";
    stream << request;
    stream << name;
    stream << publicKey;
    qDebug() << "key: exp = " << publicKey.get_exp().get_str(16).c_str() << ", mod = " << publicKey.get_module().get_str(16).c_str();

    serverSocket.waitForReadyRead();
    QString answer;
    stream >> answer;
    qDebug() << answer;
}

void Client::connectToPeer(QString name) {
    QDataStream stream(&serverSocket);

    QString request = "REQ_GET_PEER_BY_NAME";
    stream << request;

    stream << name;

    if (serverSocket.waitForReadyRead()) {
        QString address;
        stream >> address;

        RsaKey key;
        stream >> key;

        qDebug() << "another user info:";
        qDebug() << "\taddress: " << address;
        qDebug() << "\tkey: " << "exp = " << key.get_exp().get_str(16).c_str() << ", mod = " << key.get_module().get_str(16).c_str();

        //TODO: client port, arg #5
        Connection* friendConnection = new Connection(0, this, false, address, 8091);
        QThread* thread = new QThread();
        thread->start();
        friendConnection->moveToThread(thread);
        QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    }
}

void Client::getAllClients() {
    QDataStream stream(&serverSocket);
    QString req = "REQ_GET_ALL_CLIENTS";
    stream << req;

    serverSocket.waitForReadyRead();
    quint32 count;
    stream >> count;
    qDebug() << "clients" << count;
    for (int i = 0; i < count; i++) {
        QString name;
        stream >> name;
        qDebug() << name;
        connectToPeer(name);
    }
}

RsaKey Client::getPublicKey() {
    return publicKey;
}

QString Client::getName() {
    return name;
}

Client::~Client() {
    serverSocket.close();
}
