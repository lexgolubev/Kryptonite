#include "connection.h"

#include <QtNetwork>
#include "rsa/rsakey.h"

Connection::Connection(QObject *parent, Client* client, bool connected, QString address, int port)
    : QTcpSocket(parent)
{
    this->client = client;
    if (!connected) {
        connectToPeer(address, port);
    }
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

void Connection::processReadyRead()
{
    QDataStream stream(this);
    do {
        QString request;
        stream >> request;
        if (request == "REQUEST_CONNECT") {
            onRequestConnect();
        } else if (request == "MESSAGE") {
            onMessageRecivied();
        } else {
        }
    } while (bytesAvailable() > 0);
}

bool Connection::onRequestConnect() {
    QDataStream stream(this);

    QString name;
    stream >> name;
    qDebug() << "user" << name << "try to connect";

    RsaKey key;
    stream >> key;

    stream << client->getPublicKey();

    waitForReadyRead();
    QString key_str;
    stream >> key_str;

    twofish_key.set_str(key_str.toStdString().c_str(), 16);
    twofish_key = Rsa::crypt(twofish_key, client->getPrivateKey());
    twofish = new Twofish(twofish_key, 128);

    QString answer = "REQUEST_CONNECT_OK";
    stream << answer;

    waitForBytesWritten();
    client->addConnection(name, this);
    qDebug() << "connected";
    return true;
}

bool Connection::connectToPeer(QString address, int port) {
    twofish_key = Twofish::generateKey();

    this->connectToHost(address, port);
    waitForConnected(5000);
    QDataStream friend_stream(this);

    QString request = "REQUEST_CONNECT";
    friend_stream << request;
    friend_stream << client->getName();
    friend_stream << client->getPublicKey();

    waitForReadyRead(5000);
    RsaKey friend_public_key;
    friend_stream >> friend_public_key;

    QString s = Rsa::crypt(twofish_key, friend_public_key).get_str(16).c_str();
    friend_stream << s;

    waitForReadyRead(5000);
    QString answer;
    friend_stream >> answer;
    if (answer == "REQUEST_CONNECT_OK") {
        twofish = new Twofish(twofish_key, 128);
        qDebug() << "connected";
        return true;
    } else {
        qDebug() << "connection failed";
    }
    return false;
}

bool Connection::sendMessage(QString msg) {
    QDataStream stream(this);

    QString request = "MESSAGE";
    stream << request;
//    stream << msg;
    QString encrypted = twofish->encrypt_str(msg.toStdString().c_str()).get_str(16).c_str();
    stream << encrypted;

    waitForReadyRead(5000);
    QString answer;
    stream >> answer;
    if(answer == "MESSAGE_DELIVERED") {
        return true;
    }
    return false;
}

void Connection::onMessageRecivied() {
    QDataStream stream(this);

    QString msg;
    stream >> msg;
    mpz_class mpz_msg;
    mpz_msg.set_str(msg.toStdString().c_str(), 16);
    QString decrypted = twofish->decrypt_str(mpz_msg);

    QString request = "MESSAGE_DELIVERED";
    stream << request;

    waitForBytesWritten();
//    emit messageRecivied(msg);
    qDebug() << "new message:";
    qDebug() << decrypted;
//    qDebug() << msg;
}

QString Connection::getPeerName() {
    return peerName;
}
