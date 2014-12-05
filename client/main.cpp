#include "mainwindow.h"
#include <QApplication>
#include "client.h"
#include "../algo/rsa/rsakeygenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    RsaKeyGenerator gen;
    gen.generate(1024);
    QString name;
    int port;
    if (a.arguments().size() >= 3) {
        name = a.arguments().at(1);
        port = a.arguments().at(2).toInt();
    } else {
        name = "user";
        port = 8091;
    }
    Client client(name, gen.get_public_key(), gen.get_private_key(), port);

    return a.exec();
}
