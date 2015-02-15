#include "keyeventfilter.h"

KeyEventFilter::KeyEventFilter(MainWindow* parent) : parent(parent) {
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
                parent->sendMessage();
            } else {
                parent->addNewLine();
            }
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

KeyEventFilter::~KeyEventFilter() {
}
