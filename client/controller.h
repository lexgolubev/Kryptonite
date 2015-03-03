#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtGlobal>
#include <QObject>
#include <QStringList>

#include "mainwindow.h"
#include "serverconnectionthread.h"
#include "client.h"
#include "rsa/rsakeygenerator.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(QStringList args);
    ~Controller();
private:
    MainWindow* window;
    ServerConnectionThread* server;
};

#endif // CONTROLLER_H
