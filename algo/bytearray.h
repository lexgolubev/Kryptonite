#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <QVector>
#include <QString>

class ByteArray : public QVector<quint8>
{
public:
    ByteArray(QString str) {
        const quint8* byte_data = (const quint8*)str.toStdString().c_str();
        for (int i = 0; i < str.length() * 2; i++) {
            this->push_back(byte_data[i]);
        }
    }

    ByteArray() {

    }

    operator QString() {
        QString str;
        for (int i = 0; i < this->size(); i++) {
            str += this[i];
        }
        return str;
    }
};

#endif // BYTEARRAY_H
