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
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void sendMessage();

    void addNewLine();

    void on_sendButton_clicked();

public slots:
    void addUser(QString name);

    void recieveMessage(QString user, QString message);

signals:
    void connectToServer();
    void sendMessage(QString user, QString message);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
