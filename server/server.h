#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QHostAddress>
#include <QMap>
#include "connection.h"
#include "clientinfo.h"
#include "rsa/rsakey.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(int port, QObject *parent = 0);

    ~Server();

signals:
    void connected(Connection *connection);

private slots:
    void onRequestConnect();
    void onRequestGetAllClients();
    void onRequestGetPeerByName();
    void onConnectionDisconnected();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QMap<QString, ClientInfo> activeClients;

    bool addClient(QString name, const ClientInfo& info);
};

#endif // SERVER_H
