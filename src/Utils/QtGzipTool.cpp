#include "QtGzipTool.h"
#include <QtCore/QDebug>
#include <QtCore/QString>


QT_USE_NAMESPACE

namespace Huobi {
    QtGzipTool::QtGzipTool(QObject *parent) :
            QObject(parent) {
    }

    QtGzipTool::~QtGzipTool() {

    }

    QByteArray QtGzipTool::gzipUnCompress(QByteArray &src) {
        QByteArray outBuffer;
        z_stream strm;
        strm.zalloc = NULL;
        strm.zfree = NULL;
        strm.opaque = NULL;

        strm.avail_in = src.size();
        strm.next_in = (Bytef *) src.data();

        const int buff_size = 1024 * 100;

        int err = -1;
        err = inflateInit2(&strm, MAX_WBITS + 16);
        if (err == Z_OK) {
            while (true) {
                char buffer[buff_size] = {0};
                strm.avail_out = buff_size;
                strm.next_out = (Bytef *) buffer;
                int code = inflate(&strm, Z_FINISH);
                outBuffer.append(buffer, buff_size - strm.avail_out);
                if (Z_STREAM_END == code || Z_OK != code) {
                    break;
                }
            }
        }
        inflateEnd(&strm);
        return outBuffer;
    }
}



