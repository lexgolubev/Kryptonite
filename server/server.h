#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QVector>
#include <QMap>
#include "connection.h"
#include "clientinfo.h"
#include "../algo/rsa/rsakey.h"

class Connection;

class Server : public QTcpServer
{
    Q_OBJECT
private:
    const int PORT = 8090;
    QMap<QString, ClientInfo> activeClients;
public:
    Server(QObject *parent = 0);
    bool addClient(QString name, const ClientInfo& info);
    int size();
    QList<QString> clients();
    ClientInfo getPeerByName(QString name);

    ~Server();
signals:
    void newConnection(Connection *connection);

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER_H
