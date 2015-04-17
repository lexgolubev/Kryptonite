#ifndef DHT_H
#define DHT_H

#include <QBitArray>
#include <QObject>

class DHT
{
public:
    DHT();

    virtual void store(QBitArray key, QObject value) = 0;

    virtual QObject lookup(QBitArray key) = 0;

    virtual ~DHT();
};

#endif // DHT_H
