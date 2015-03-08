#include <QDebug>
#include "server.h"
#include <QCoreApplication>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "Server started\n";
    int port = app.arguments().at(1).toInt();
    Server server(port);

    return app.exec();
}

