#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <QVector>
#include <QString>

class ByteArray : public QVector<quint8>
{
public:
    ByteArray(QString str) {
        const quint8* byte_data = (const quint8*)str.toStdString().c_str();
        for (int i = 0; i < str.length(); i++) {
            this->push_back(byte_data[i]);
        }
    }

    ByteArray(quint8* str, int length) {
        for (int i = 0; i < length; i++) {
            this->push_back(str[i]);
        }
    }

    ByteArray() {

    }

    operator QString() {
        QString str = "";
        for (int i = 0; i < this->size(); i++) {
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
        int length = this->size();
        int k = (length + 3) / 4;
        for (int i = 0; i < k; i++) {
            //swap 4*i with 4*i+3
            quint8 tmp = this->at(4 * i);
            this->replace(4 * i, this->at(4 *i + 3));
            this->replace(4 * i + 3, tmp);
            //swap 4*i+1 with 4*i+2
            tmp = this->at(4 * i + 1);
            this->replace(4 * i + 1, this->at(4 *i + 2));
            this->replace(4 * i + 2, tmp);
        }
    }
};

#endif // BYTEARRAY_H
