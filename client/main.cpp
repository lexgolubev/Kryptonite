#include <QApplication>
#include "mainwindow.h"
#include "client.h"
#include "rsa/rsakeygenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    RsaKeyGenerator gen;
    gen.generate(1024);

    QString serverIp = "192.168.0.104";
    int serverPort = 8090;
    QString name = "user";
    int localPort = 8091;
    if (a.arguments().size() >= 5) {
        serverIp = a.arguments().at(1);
        serverPort = a.arguments().at(2).toInt();
        name = a.arguments().at(3);
        localPort = a.arguments().at(4).toInt();
    }
    Client* client = new Client(name, gen.get_public_key(), gen.get_private_key(), localPort, serverIp, serverPort);

    return a.exec();
}
