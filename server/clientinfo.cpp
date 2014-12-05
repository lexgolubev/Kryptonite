#include "clientinfo.h"

ClientInfo::ClientInfo() {

}

ClientInfo::ClientInfo(const QHostAddress& hostAddress, const RsaKey& publicKey) :
    hostAddress(hostAddress), publicKey(publicKey) {

}

RsaKey& ClientInfo::getPublicKey() {
    return publicKey;
}

QHostAddress& ClientInfo::getHostAddress() {
    return hostAddress;
}

ClientInfo::~ClientInfo() {
}
