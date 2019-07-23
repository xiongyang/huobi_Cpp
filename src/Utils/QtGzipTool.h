#ifndef QTGZIPTOOL_H
#define QTGZIPTOOL_H

#include <QtCore/QObject>
#include <QByteArray>
#include <iostream>
#include "zlib.h"


using namespace std;

namespace Huobi {
    class QtGzipTool : public QObject {
    Q_OBJECT
    public:
        explicit QtGzipTool(QObject *parent = nullptr);
        ~QtGzipTool();

    public:
        static QByteArray gzipUnCompress(QByteArray &src);
    };
}
#endif // QTGZIPTOOL_H
