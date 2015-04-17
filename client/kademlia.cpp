#include "kademlia.h"

Kademlia::Kademlia(QBitArray nodeId) {

}

int Kademlia::distance(const QBitArray& first, const QBitArray& second) {
    return (first ^ second).count(true);
}

void Kademlia::store(QBitArray key, QObject value) {

}

QObject Kademlia::lookup(QBitArray key) {
}

Kademlia::~Kademlia() {

}

