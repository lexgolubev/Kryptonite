#include <iostream>
#include "server.h"
#include <QCoreApplication>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    std::cout << "Server started\n";
    Server server;
    server.waitForNewConnection(30000);
    return app.exec();
}

