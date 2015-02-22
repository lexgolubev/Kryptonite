#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString serverIp, int serverPort,
                       QString name, int localPort) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    KeyEventFilter* filter = new KeyEventFilter(this);
    ui->listWidgetUsers->installEventFilter(filter);
    ui->textEditMessage->installEventFilter(filter);

    RsaKeyGenerator gen;
    gen.generate(1024);

    client = new Client(name, gen.get_public_key(), gen.get_private_key(), localPort, serverIp, serverPort);
    QThread* thread = new QThread();
    thread->start();
    client->moveToThread(thread);
    connect(this, SIGNAL(connectToServer()), client, SLOT(connectToServer()));
    emit connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::sendMessage() {
//    QString user = ui->listWidgetUsers->currentItem()->text();
    QString message = ui->textEditMessage->toPlainText();
    //TODO: send msg
    ui->textEditMessage->setPlainText("");
    return true;
}

void MainWindow::addNewLine() {
    QString text = ui->textEditMessage->toPlainText();
    text += "\n";
    ui->textEditMessage->setPlainText(text);
    ui->textEditMessage->moveCursor(QTextCursor::End);
}
