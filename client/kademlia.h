#ifndef KADEMLIA_H
#define KADEMLIA_H

#include "dht.h"
#include <QBitArray>

class Kademlia : public DHT
{
public:
    Kademlia(QBitArray nodeId);

    virtual void store(QBitArray key, QObject value);

    virtual QObject lookup(QBitArray key);

    ~Kademlia();

private:
    int distance(const QBitArray& first, const QBitArray& second);
};

#endif // KADEMLIA_H
