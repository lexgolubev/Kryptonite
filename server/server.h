#ifndef SERVER_H
#define SERVER_H

#include <QtGlobal>
//#include <QObject>
#include <QTcpServer>
#include <QVector>
#include <QTcpSocket>
#include <QMap>
#include "clientinfo.h"
#include "../algo/rsa/rsakey.h"

class Server : public QObject
{
    Q_OBJECT

public:
    Server();

    QMap<QString, ClientInfo> activeClients;

    ~Server();

public slots:
    void newConnection();
private:
    QTcpServer server;
};

#endif // SERVER_H
