#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <QDataStream>
#include <QVector>
#include <QString>

class ByteArray : public QVector<quint8>
{
public:
    int msg_length;

    ByteArray(QString str) {
        const quint8* byte_data = (const quint8*)str.toStdString().c_str();
        for (int i = 0; i < str.length(); i++) {
            this->push_back(byte_data[i]);
        }
        msg_length = str.length();
    }

    ByteArray(quint8* str, int length) {
        for (int i = 0; i < length; i++) {
            this->push_back(str[i]);
        }
    }

    ByteArray() {

    }

    operator QString() {
        return toStr();
    }

    QString toStr() {
        QString str = "";
        for (int i = 0; i < msg_length && i < this->length(); i++) {
            quint8 q = this->at(i);
            str += (char)q;
        }
        return str;
    }

    operator char*() {
        quint8* data = this->data();
        return (char*) data;
    }

    void toLittleEndian() {
        quint8* data = this->data();
        int length = this->size();
        int k = (length + 3) / 4;
        for (int i = 0; i < k; i++) {
            //swap 4i and 4i+3
            quint8 tmp = data[4 * i];
            data[4 * i] = data[4 * i + 3];
            data[4 * i + 3] = tmp;
            //swap 4i+1 and 4i+2
            tmp = data[4 * i + 1];
            data[4 * i + 1] = data[4 * i + 2];
            data[4 * i + 2] = tmp;
        }
    }

    void cut() {
        for (int i = msg_length; i < this->length(); i++) {
            this->removeAt(i);
        }
    }

    static void read(QDataStream& in, ByteArray& arr) {
        in >> arr.msg_length;
        in >> arr;
    }

    static void write(QDataStream& out, const ByteArray& arr) {
        out << arr.msg_length;
        out << arr;
    }

};



#endif // BYTEARRAY_H
