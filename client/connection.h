#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtNetwork>
#include <QString>
#include "rsa/rsa.h"
#include "twofish/twofish.h"

class Connection : public QTcpSocket
{
    Q_OBJECT

    static constexpr int PING_TIMEOUT = 10000;

public:
    Connection(QObject *parent);
    bool connectToPeer(QString address, int port);
    bool sendMessage(QString msg);

    QString getPeer();

    void setPeer(QString peer);
    void setName(QString name);
    void setFriendRsaKey(RsaKey key);
    void setOwnPublicKey(RsaKey key);
    void setOwnPrivateKey(RsaKey key);

signals:
    void messageRecivied(QString msg);
    void addConnection(QString name, Connection* connection);

private slots:
    void processReadyRead();

private:
    bool onRequestConnect();
    void onMessageRecivied();

private:
    mpz_class twofish_key;
    Twofish* twofish;

    QString peer;
    QString name;
    RsaKey friendRsaKey;
    RsaKey ownPublicKey;
    RsaKey ownPrivateKey;
};

#endif
