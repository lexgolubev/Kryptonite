#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    KeyEventFilter* filter = new KeyEventFilter(this);
    ui->listWidgetUsers->installEventFilter(filter);
    ui->textEditMessage->installEventFilter(filter);
    connect(filter, SIGNAL(sendMessage()), this, SLOT(sendMessage()));
    connect(filter, SIGNAL(addNewLine()), this, SLOT(addNewLine()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage() {
    QString user = "user1";
    if (ui->listWidgetUsers->currentRow() >= 0) {
        user = ui->listWidgetUsers->currentItem()->text();
    }
    QString message = ui->textEditMessage->toPlainText();
    if (message.length() != 0) {
        emit sendMessage(user, message);
        ui->textEditMessage->setPlainText("");
        recieveMessage("I", message);
    }
}

void MainWindow::addNewLine() {
    QString text = ui->textEditMessage->toPlainText();
    text += "\n";
    ui->textEditMessage->setPlainText(text);
    ui->textEditMessage->moveCursor(QTextCursor::End);
}

void MainWindow::addUser(QString name) {
    if (ui->listWidgetUsers->findItems(name, Qt::MatchExactly).size() == 0) {
        ui->listWidgetUsers->addItem(name);
    }
}

void MainWindow::recieveMessage(QString user, QString message) {
    QString text = user + ": " + message;
    ui->textEditHistory->append(text);
}
