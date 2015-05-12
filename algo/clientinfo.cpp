#include "clientinfo.h"

ClientInfo::ClientInfo() {

}

ClientInfo::ClientInfo(const QHostAddress& hostAddress, int port, const RsaKey& publicKey, QString name) :
    hostAddress(hostAddress), publicKey(publicKey), port(port), name(name) {

}

RsaKey ClientInfo::getPublicKey() const {
    return publicKey;
}

void ClientInfo::setPublicKey(const RsaKey &key) {
    this->publicKey = key;
}

QHostAddress ClientInfo::getHostAddress() const {
    return hostAddress;
}

void ClientInfo::setHostAddress(const QHostAddress &hostAddress) {
    this->hostAddress = hostAddress;
}

int ClientInfo::getPort() const {
    return port;
}

void ClientInfo::setPort(int port) {
    this->port = port;
}

QString ClientInfo::getName() const {
    return name;
}

void ClientInfo::setName(QString name) {
    this->name = name;
}

ClientInfo::~ClientInfo() {
}

QDataStream& operator <<(QDataStream& out, const ClientInfo& info) {
    out << info.hostAddress;
    out << info.port;
    out << info.publicKey;
    out << info.name;
    return out;
}

QDataStream& operator >>(QDataStream& in, ClientInfo& info) {
    in >> info.hostAddress;
    in >> info.port;
    in >> info.publicKey;
    in >> info.name;
    return in;
}
