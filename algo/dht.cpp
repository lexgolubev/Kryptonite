#include "dht.h"

Dht::Dht() : bootstrapSocket(nullptr)
{
}

void Dht::setBootstrapServer(QString ip, int port) {
    if (bootstrapSocket != nullptr) {
        bootstrapSocket->close();
    }
    bootstrapSocket = new QTcpSocket();
    bootstrapSocket->connectToHost(ip, port);
}

QList<QPair<QString, int>> Dht::getListOfPosibleUsers(int size) {
    QDataStream stream(bootstrapSocket);
    stream << DhtCode::REQUEST_GET_PEERS;
    stream << size;
    bootstrapSocket->waitForReadyRead();

    QList<QPair<QString, int>> result;
    QString end = "END";
    QString ip;
    int port;
    stream >> ip;
    while (ip != end) {
        stream >> port;
        result.push_back(QPair<QString, int>(ip, port));
        stream >> ip;
    }
    return result;
}

void Dht::connectToPosibleUsers() {
    QList<QPair<QString, int>> list = getListOfPosibleUsers(CONNECTION_COUNT);
    for (auto it = list.begin(); it != list.end(); ++it) {
        QTcpSocket* connection = new QTcpSocket();
        connection->connectToHost(it->first, it->second);
        if (connection->waitForConnected()) {
            servers.push_back(connection);
        } else {
            connection->deleteLater();
        }
    }
}

QList<QString> Dht::getAllClients() {
    QList<QString> clients;
    foreach (QTcpSocket* socket, servers) {
        QDataStream stream(socket);
        stream << DhtCode::REQUEST_GET_ALL_CLIENTS;
        socket->waitForBytesWritten();
        int count;
        stream >> count;
        for (int i = 0; i < count; i++) {
            QString name;
            stream >> name;
            clients.push_back(name);
        }
    }
    return clients;
}

ClientInfo Dht::getInfo(QString name) {
    ClientInfo info;
    foreach (QTcpSocket* socket, servers) {
        QDataStream stream(socket);

        stream << DhtCode::REQUEST_GET_USER_INFO;
        socket->waitForReadyRead();

        DhtCode answer;
        stream >> answer;
        if (answer == DhtCode::ANSWER_GET_USER_INFO) {
            stream >> info;
        }
    }
    return info;
}

bool Dht::connectToUser(const ClientInfo& userInfo) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost(userInfo.getHostAddress(), userInfo.getPort());
    if (socket->waitForConnected()) {
        activeUsers[userInfo.getName()] = socket;
        return true;
    } else {
        return false;
    }
}

Dht::~Dht()
{

}

QDataStream& operator <<(QDataStream& out, const DhtCode& code) {
    out << static_cast<std::underlying_type<DhtCode>::type>(code);
    return out;
}

QDataStream& operator >>(QDataStream& in, DhtCode& code) {
    int value;
    in >> value;
    code = static_cast<DhtCode>(value);
    return in;
}
