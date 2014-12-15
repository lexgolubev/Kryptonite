#include "clientinfo.h"

ClientInfo::ClientInfo() {

}

ClientInfo::ClientInfo(const QHostAddress& hostAddress, int port, const RsaKey& publicKey) :
    hostAddress(hostAddress), publicKey(publicKey), port(port) {

}

RsaKey& ClientInfo::getPublicKey() {
    return publicKey;
}

QHostAddress& ClientInfo::getHostAddress() {
    return hostAddress;
}

int ClientInfo::getPort() {
    return port;
}

ClientInfo::~ClientInfo() {
}
