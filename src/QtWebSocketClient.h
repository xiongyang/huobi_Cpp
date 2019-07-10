#ifndef QTWEBSOCKETCLIENT_H
#define QTWEBSOCKETCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <iostream>
#include "Utils/JsonDocument.h"
#include "Utils/ChannelParser.h"
#include "Huobi/PriceDepthEvent.h"
#include "Utils/JsonWrapper.h"
#include "Utils/TimeService.h"
#include <QtCore/QJsonObject>
#include "Utils/QtGzipTool.h"
#include "Utils/QtJsonTool.h"


using namespace std;

QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace Huobi {
    class QtWebSocketClient : public QObject {
    Q_OBJECT
    public:
        explicit QtWebSocketClient(const QUrl &url, QList<QString> topics, QObject *parent = nullptr);

        ~QtWebSocketClient();

    private Q_SLOTS:

        void onConnected();

        void onBinaryMessageReceived(QByteArray message);

    Q_SIGNALS:
        void received(QJsonObject jsonObject);

    private:
        void sendPongOnMarketLine(QJsonObject& jsonObject);
        void sendPongOnTradingLine(QJsonObject& jsonObject);
        void sendTopic();

    private:
        QWebSocket m_webSocket;
//        QtGzipTool *m_gzipTool;
//        QtJsonTool *m_jsonTool;
        QList<QString> m_topics;
    };
}
#endif // QTWEBSOCKETCLIENT_H
