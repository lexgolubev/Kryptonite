#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QtGlobal>
#include <QKeyEvent>
#include <QDebug>

class KeyEventFilter : public QObject
{
    Q_OBJECT
public:
    KeyEventFilter(QObject* parent = 0);

    bool eventFilter(QObject *obj, QEvent *event);

    ~KeyEventFilter();

signals:
    void sendMessage();
    void addNewLine();
};

#endif // KEYEVENTFILTER_H
