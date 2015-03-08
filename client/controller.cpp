#include "controller.h"

Controller::Controller(QStringList args)
{
    QString serverIp = "127.0.0.1";
    int serverPort = 9000;
    QString name = "user";
    int localPort = 8091;
    if (args.size() >= 5) {
        serverIp = args[1];
        serverPort = args[2].toInt();
        name = args[3];
        localPort = args[4].toInt();
    }

    window = new MainWindow(0);
    window->show();

    server = new ServerConnectionThread(name, localPort, serverIp, serverPort);
    connect(server, SIGNAL(addUser(QString)), window, SLOT(addUser(QString)));
    connect(server, SIGNAL(recieveMessage(QString,QString)), window, SLOT(recieveMessage(QString,QString)));
    connect(window, SIGNAL(sendMessage(QString,QString)), server, SLOT(onSendMessage(QString, QString)));
    server->start();

}

Controller::~Controller()
{

}

