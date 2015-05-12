#ifndef DHT_H
#define DHT_H

#include <QPair>
#include <QtNetwork>
#include "clientinfo.h"

//implement dht (like Gnutella)

class Dht : public QObject
{
    Q_OBJECT
private:
    static constexpr int CONNECTION_COUNT = 2;
public:
    Dht();

    //set bootsrap server, must be called before getListOfPosibleUsers
    void setBootstrapServer(QString ip, int port);

    //requests from bootstrap server list of posible connected users
    QList<QPair<QString, int>> getListOfPosibleUsers(int size);

    void connectToPosibleUsers();

    QList<QString> getAllClients();

    ClientInfo getInfo(QString name);

    bool connectToUser(const ClientInfo& userInfo);

    ~Dht();
private:
    QTcpSocket* bootstrapSocket;

    QList<QTcpSocket*> servers;

    QMap<QString, QTcpSocket*> activeUsers;
};

enum DhtCode {
    REQUEST_GET_PEERS,
    REQUEST_GET_ALL_CLIENTS,
    REQUEST_GET_USER_INFO,
    ANSWER_END,
    ANSWER_GET_PEERS,
    ANSWER_GET_ALL_CLIENTS,
    ANSWER_GET_USER_INFO
};

QDataStream& operator <<(QDataStream& out, const DhtCode& code);
QDataStream& operator >>(QDataStream& in, DhtCode& code);


#endif // DHT_H
