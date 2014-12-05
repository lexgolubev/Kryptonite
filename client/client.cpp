#include "client.h"
#include <QThread>
#include "unistd.h"

Client::Client(QString name, RsaKey public_key, RsaKey private_key, int port) {
       //: public_key(public_key), private_key(private_key), name(name) {
    this->public_key = public_key;
    this->private_key = private_key;
    this->name = name;
    server = new Server(0, this, port);

    QHostAddress server_address(SERVER_IP);
    server_socket.connectToHost(server_address, SERVER_PORT);

    if (server_socket.waitForConnected()) {
        qDebug() << "connected successfully";
    } else {
        qDebug() << "connection fail";
    }

    send_init_req(public_key, name);

    getAllClients();
}

void Client::send_init_req(RsaKey public_key, QString name)
{
    QDataStream stream(&server_socket);
    QString request = "REQ_INIT";
    stream << request;
    stream << name;
    stream << public_key;
    qDebug() << "key: exp = " << public_key.get_exp().get_str(16).c_str() << ", mod = " << public_key.get_module().get_str(16).c_str();

    server_socket.waitForReadyRead();
    QString answer;
    stream >> answer;
    qDebug() << answer;
}

void Client::connectToPeer(QString name) {
    QDataStream stream(&server_socket);

    QString request = "REQ_GET_PEER_BY_NAME";
    stream << request;

    stream << name;

    if (server_socket.waitForReadyRead()) {
        QString address;
        stream >> address;

        RsaKey key;
        stream >> key;

        qDebug() << "another user info:";
        qDebug() << "\taddress: " << address;
        qDebug() << "\tkey: " << "exp = " << key.get_exp().get_str(16).c_str() << ", mod = " << key.get_module().get_str(16).c_str();

        Connection* friend_connection = new Connection(0, this, false, address, CLIENT_PORT);
        QThread* thread = new QThread();
        thread->start();
        QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        friend_connection->moveToThread(thread);
    }
}

void Client::getAllClients() {
    QDataStream stream(&server_socket);
    QString req = "REQ_GET_ALL_CLIENTS";
    stream << req;

    server_socket.waitForReadyRead();
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
    return public_key;
}

QString Client::getName() {
    return name;
}

Client::~Client() {
    server_socket.close();
}
