#include <QApplication>
#include <iostream>
#include "mainwindow.h"
#include "client.h"
#include "rsa/rsakeygenerator.h"

#define MAX_MSG_LENGTH 100

class ConsoleReaderThread : public QThread {
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        while (true) {
            std::string cmd;
            std::cin >> cmd;
            if (cmd.compare("ls") == 0) {
                QList<QString> peers = client->getAllClients();
                if (peers.size() != 0) {
                    foreach (QString peer, peers) {
                        std::cout << peer.toStdString() << std::endl;
                    }
                } else {
                    std::cout << "no clients\n";
                }
            } else if (cmd.compare("connect") == 0) {
                std::string dest;
                std::cin >> dest;
                client->connectToPeer(QString::fromStdString(dest));
            } else if (cmd.compare("send") == 0) {
                std::string dest;
                std::cin >> dest;
                char msg[MAX_MSG_LENGTH];
                std::cin.getline(msg, MAX_MSG_LENGTH);
                client->sendMessage(QString::fromStdString(dest), msg);
            } else if (cmd.compare("exit") == 0) {
                std::cout << "bye" << std::endl;
                break;
            }
//            cmd = "";
         }
    }
private:
    Client* client;
public:
    ConsoleReaderThread(Client* client) {
        this->client = client;
    }
};

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
//    Client* client = new Client(name, gen.get_public_key(), gen.get_private_key(), localPort, serverIp, serverPort);

//    QThread* thread = new ConsoleReaderThread(client);
//    thread->start();

    return a.exec();
}

#include "main.moc"
