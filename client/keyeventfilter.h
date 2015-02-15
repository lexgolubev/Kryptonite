#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QtGlobal>
#include <QKeyEvent>
#include "mainwindow.h"
#include <QDebug>

class MainWindow;

class KeyEventFilter : public QObject
{
public:
    KeyEventFilter(MainWindow* parent);

    bool eventFilter(QObject *obj, QEvent *event);

    ~KeyEventFilter();

private:
    MainWindow* parent;
};

#endif // KEYEVENTFILTER_H
