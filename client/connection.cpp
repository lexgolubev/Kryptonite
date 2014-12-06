#include "connection.h"

#include <QtNetwork>
#include "rsa/rsakey.h"

Connection::Connection(QObject *parent, Client* client, bool connected, QString address, int port)
    : QTcpSocket(parent)
{
    this->client = client;
    if (!connected) {
        init_friend(address, port);
    }
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

void Connection::processReadyRead()
{
    qDebug() << "ready";
    QDataStream stream(this);
    do {
        QString request;
        stream >> request;
        if (request == "REQ_INIT") {
            req_init();
            QString msg = "Hello lex!";
            send_msg(msg);
        } else if (request == "REQ_MSG") {
            req_msg();
        } else {

        }
    } while (bytesAvailable() > 0);
}

void Connection::req_init() {
    QDataStream stream(this);

    QString name;
    stream >> name;
    qDebug() << name;

    qDebug() << "user" << name << "try to connect";

    RsaKey key;
    stream >> key;
    qDebug() << "exp = " << key.get_exp().get_str(16).c_str() << ", mod = " << key.get_module().get_str(16).c_str();

    stream << client->getPublicKey();

    waitForReadyRead();
    QString answer;
    stream >> answer;
    qDebug() << "chanel init" << answer;
}

void Connection::init_friend(QString address, int port) {
    qDebug() << "init friend";
    this->connectToHost(address, port);
    if (this->waitForConnected()) {
        QDataStream friend_stream(this);

        QString request = "REQ_INIT";
        friend_stream << request;
        friend_stream << client->getName();
        friend_stream << client->getPublicKey();

        waitForReadyRead();

        RsaKey friend_public_key;
        friend_stream >> friend_public_key;
        qDebug() << "\tkey: " << "exp =" << friend_public_key.get_exp().get_str(16).c_str() << ", mod =" << friend_public_key.get_module().get_str(16).c_str();

        QString answer = "CHANEL_INIT";
        friend_stream << answer;
        waitForBytesWritten();
        qDebug() << "chanel init";
    }
}

bool Connection::send_msg(QString msg) {
    QDataStream stream(this);

    QString request = "REQ_MSG";
    stream << request;
    stream << msg;

    waitForReadyRead();

    QString answer;
    stream >> answer;

    qDebug() << "send msg ans" << answer;
    return answer == "REQ_MSG_OK";
}

void Connection::req_msg() {
    QDataStream stream(this);

    QString msg;
    stream >> msg;
    qDebug() << "new msg:" << msg;

    QString request = "REQ_MSG_OK";
    stream << request;

    waitForBytesWritten();
    qDebug() << "msg recivied";
}
