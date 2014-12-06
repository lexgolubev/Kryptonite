#include <iostream>
#include "server.h"
#include <QCoreApplication>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    std::cout << "Server started\n";
    int port = app.arguments().at(1).toInt();
    Server* server = new Server(port);

    return app.exec();
}

