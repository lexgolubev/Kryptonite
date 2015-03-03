#include "controller.h"

Controller::Controller(QStringList args)
{
    RsaKeyGenerator gen;
    gen.generate(1024);

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

    server = new ServerConnectionThread(name, gen.get_public_key(), gen.get_private_key(),
                                        localPort, serverIp, serverPort);
    connect(server, SIGNAL(addUser(QString)), window, SLOT(addUser(QString)));
    connect(server, SIGNAL(recieveMessage(QString,QString)), window, SLOT(recieveMessage(QString,QString)));
    connect(window, SIGNAL(sendMessage(QString,QString)), server, SLOT(onSendMessage(QString, QString)));
    server->start();

}

Controller::~Controller()
{

}

