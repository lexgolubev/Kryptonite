#include "keyeventfilter.h"

KeyEventFilter::KeyEventFilter(QObject* parent) : QObject(parent) {
}

bool KeyEventFilter::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* eventKey = static_cast<QKeyEvent*>(event);
        int key = eventKey->key();
        if  (key == Qt::Key_Enter || key == Qt::Key_Return) {
            Qt::KeyboardModifiers modifiers = eventKey->modifiers();
            bool shift = modifiers.testFlag(Qt::KeyboardModifier::ShiftModifier);
            if (!shift) {
                emit sendMessage();
            } else {
                emit addNewLine();
            }
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

KeyEventFilter::~KeyEventFilter() {
}
