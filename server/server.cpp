#include "server.h"
#include <QDebug>

Server::Server() {
    QObject::connect(&server, &QTcpServer::newConnection,
                     this, &Server::newConnection);
    server.listen(QHostAddress::LocalHost);
}

void Server::newConnection() {
    QTcpSocket* incoming =  server.nextPendingConnection();
    QHostAddress address =  incoming->peerAddress();
    QDataStream stream(incoming);

    QString name;
    RsaKey key;
    stream << name;
    stream << key;
    qDebug() << name;
    qDebug() << "exp = " << key.get_exp().get_str(16).c_str() << ", mod = " << key.get_module().get_str(16).c_str();

    if (!activeClients.contains(name)) {
        ClientInfo client(address, key);
        activeClients.insert(name, client);
    } else {
        qDebug() << "This name already used. Please, select another name";
    }

    QString clientName;
    while (incoming->state() == QAbstractSocket::SocketState::ConnectedState) {
        stream >> clientName;
        stream << activeClients.take(clientName).getPublicKey();
    }
}

Server::~Server() {
    server.close();
    activeClients.clear();
}
