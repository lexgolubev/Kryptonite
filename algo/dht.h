#ifndef DHT_H
#define DHT_H

#include <QPair>
#include <QtNetwork>

//implement simple dht (as in Gnutella)

class Dht
{
public:
    Dht();

    //set bootsrap server, must be called before getListOfPosibleUsers
    void setBootstrapServer(QString ip, int port);

    //requests from bootstrap server list of posible connected users
    QList<QPair<QString, int>> getListOfPosibleUsers(int size);

    ~Dht();
private:
    QTcpSocket* bootstrapSocket;
};

#endif // DHT_H
