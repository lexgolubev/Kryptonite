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
