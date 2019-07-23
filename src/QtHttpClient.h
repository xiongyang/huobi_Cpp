#ifndef QTHTTPCLIENT_H
#define QTHTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include "Utils/QtJsonTool.h"

using namespace std;

QT_FORWARD_DECLARE_CLASS(QNetworkAccessManager)

namespace Huobi {
    class QtHttpClient : public QObject {
    Q_OBJECT
    public:
        explicit QtHttpClient(QObject *parent = nullptr);

        ~QtHttpClient();

    public:
        void get(QUrl url);

        bool post(QUrl url, QByteArray &bytes);

    Q_SIGNALS:

        void received(QJsonObject jsonObject);

    public slots:
        void replyFinished(QNetworkReply *reply);

    private:
        QNetworkAccessManager m_pManager;
    };
}

#endif // QTHTTPCLIENT_H
