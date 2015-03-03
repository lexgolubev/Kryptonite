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
    QString user = "user1";//ui->listWidgetUsers->currentItem()->text();
    QString message = ui->textEditMessage->toPlainText();
    emit sendMessage(user, message);
    ui->textEditMessage->setPlainText("");
}

void MainWindow::addNewLine() {
    QString text = ui->textEditMessage->toPlainText();
    text += "\n";
    ui->textEditMessage->setPlainText(text);
    ui->textEditMessage->moveCursor(QTextCursor::End);
}

void MainWindow::addUser(QString name) {
    ui->listWidgetUsers->addItem(name);
}
