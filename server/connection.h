#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    Connection(QObject *parent);

    QString getName();
    void setName(QString name);

signals:
    void requestConnect();
    void requestGetAllClients();
    void requestGetPeerByName();
    void broadcastInfo();

private slots:
    void processReadyRead();

private:
    QString name;
};

#endif
