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
    setPeer(name);
    qDebug() << "user" << name << "try to connect";

    RsaKey key;
    stream >> key;
    setFriendRsaKey(key);

    QString key_str;
    stream >> key_str;
    twofish_key.set_str(key_str.toStdString().c_str(), 16);
    twofish_key = Rsa::crypt(twofish_key, ownPrivateKey);
    twofish = new Twofish(twofish_key, 128);

    QString answer = "REQUEST_CONNECT_OK";
    stream << answer;

    waitForBytesWritten();

    qDebug() << "connected";

    return true;
}

bool Connection::connectToPeer(QString address, int port) {
    qDebug() << "connect to " << address << ":" << port;
    twofish_key = Twofish::generateKey();

    this->connectToHost(address, port);
    if (!waitForConnected(5000)) {
        qDebug() << "failed to connect";
        return false;
    }
    this->blockSignals(true);
    QDataStream friend_stream(this);

    QString request = "REQUEST_CONNECT";
    friend_stream << request;
    friend_stream << name;
    friend_stream << ownPublicKey;

    QString s = Rsa::crypt(twofish_key, friendRsaKey).get_str(16).c_str();
    friend_stream << s;

    waitForReadyRead(10000);
    QString answer;
    friend_stream >> answer;
    this->blockSignals(false);
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

    qDebug() << "send msg: " << msg;
    ByteArray encrypted = twofish->encrypt_qstr(msg);
    ByteArray::write(stream, encrypted);

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

    ByteArray encrypted;
    ByteArray::read(stream, encrypted);

    qDebug() << "recieved: " << encrypted << "msg_length " << encrypted.msg_length;
    QString decrypted = twofish->decrypt_qstr(encrypted);

    QString request = "MESSAGE_DELIVERED";
    stream << request;

    waitForBytesWritten();
    emit messageRecivied(decrypted);
    qDebug() << "new message from:" << peer;
    qDebug() << decrypted;
}

QString Connection::getPeer() {
    return peer;
}

void Connection::setPeer(QString peerName) {
    this->peer = peerName;
}

void Connection::setFriendRsaKey(RsaKey key) {
    friendRsaKey = key;
}

void Connection::setOwnPrivateKey(RsaKey key) {
    ownPrivateKey = key;
}

void Connection::setOwnPublicKey(RsaKey key) {
    ownPublicKey = key;
}

void Connection::setName(QString name) {
    this->name = name;
}
