#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "keyeventfilter.h"
#include "client.h"
#include "rsa/rsakeygenerator.h"
#include "serverconnectionthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString serverIp = QString("127.0.0.1"),
                        int serverPort = 9000, QString name = QString("user"), int localPort = 8091);

    bool sendMessage();

    void addNewLine();

    ~MainWindow();

signals:
    void connectToServer();

private:
    Ui::MainWindow *ui;

//    Client* client;
};

#endif // MAINWINDOW_H
