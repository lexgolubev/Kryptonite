#include <QApplication>
#include <iostream>
#include "mainwindow.h"
#include "client.h"
#include "rsa/rsakeygenerator.h"

#include "controller.h"

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
    Controller ctrl(a.arguments());
    return a.exec();
}

#include "main.moc"
