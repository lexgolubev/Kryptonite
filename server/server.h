#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "connection.h"
#include "clientinfo.h"
#include "rsa/rsakey.h"

class Connection;

class Server : public QTcpServer
{
    Q_OBJECT
private:
    QMap<QString, ClientInfo> activeClients;

public:
    Server(int port, QObject *parent = 0);

    bool addClient(QString name, const ClientInfo& info);
    ClientInfo getPeerByName(QString name);
    QList<QString> clients();
    int size();

    ~Server();

signals:
    void newConnection(Connection *connection);

public slots:
    void onConnectionDisconnected();

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER_H
