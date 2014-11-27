#include "client.h"

Client::Client(QString name, RsaKey public_key, RsaKey private_key) : public_key(public_key),
        private_key(private_key) {
    QHostAddress server_address = QHostAddress(SERVER_IP);
    server_socket.connectToHost(server_address, qrand() % 10000 + 8000);
    QDataStream stream(&server_socket);
    stream >> name;
    stream >> public_key;
}

Client::~Client() {
    server_socket.close();
}
