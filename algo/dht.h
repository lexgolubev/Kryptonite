#ifndef DHT_H
#define DHT_H

#include <QtNetwork>

//implement simple dht (as in Gnutella)

class Dht
{
public:
    Dht();
    void setBootstrapServer(QString ip, int port);
    ~Dht();
};

#endif // DHT_H
