#ifndef DHT_H
#define DHT_H

#include <QPair>
#include <QtNetwork>

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

    ~Dht();
private:
    QTcpSocket* bootstrapSocket;

    QList<QTcpSocket*> servers;
};

#endif // DHT_H
