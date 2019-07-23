#include "QtWebSocketClient.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QString>
#include <QByteArray>


QT_USE_NAMESPACE

namespace Huobi {
    QtWebSocketClient::QtWebSocketClient(const QUrl &url, QList<QString> topics, QObject *parent) :
            QObject(parent) {
        m_isNeedAuth = false;

        connect(&m_webSocket, &QWebSocket::connected, this, &QtWebSocketClient::onConnected);
        m_topics = topics;
        m_webSocket.open(QUrl(url));
    }

    QtWebSocketClient::QtWebSocketClient(const QUrl &url, QList<QString> topics, QString authTopic, QObject *parent) :
            QObject(parent) {
        m_authTopic = authTopic;
        m_isNeedAuth = true;

        connect(&m_webSocket, &QWebSocket::connected, this, &QtWebSocketClient::onConnected);
        m_topics = topics;
        m_webSocket.open(QUrl(url));
    }

    QtWebSocketClient::~QtWebSocketClient() {
    }


//! [onConnected]
    void QtWebSocketClient::onConnected() {
        qDebug() << "建立连接" << endl;
        connect(&m_webSocket, &QWebSocket::binaryMessageReceived,
                this, &QtWebSocketClient::onBinaryMessageReceived);
        sendTopic();
    }
//! [onConnected]


    void QtWebSocketClient::sendPongOnTradingLine(QJsonObject &jsonObject) {
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();
        QString ts = result["ts"].toString();

        QJsonObject mjson;
        mjson.insert("op", "pong");
        mjson.insert("ts", ts);
        QString jsonString = QtJsonTool::getStringFromJsonObject(mjson);

        m_webSocket.sendTextMessage(jsonString);
    }

    void QtWebSocketClient::sendPongOnMarketLine(QJsonObject &jsonObject) {
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        QString ts = result["ping"].toString();

        QJsonObject mjson;
        mjson.insert("pong", ts);

        QString jsonString = QtJsonTool::getStringFromJsonObject(mjson);

        m_webSocket.sendTextMessage(jsonString);
    }

    void QtWebSocketClient::sendTopic() {
        if (m_isNeedAuth) {
            qDebug()<<"校验数据"<< endl;
            m_webSocket.sendTextMessage(m_authTopic);
        } else {
                    foreach (QString topic, m_topics) {
                    m_webSocket.sendTextMessage(topic);
                }
        }
    }


    void QtWebSocketClient::onBinaryMessageReceived(QByteArray message) {
        qDebug() << "收到数据" << endl;
        //解压缩
        QByteArray buffer = QtGzipTool::gzipUnCompress(message);
        //转成json对象
        QJsonObject jsonObject = QtJsonTool::getJsonObjectFromByteArray(buffer);

        //收到的数据错误或者为空
        if (jsonObject.isEmpty()) {
            qDebug() << "数据为空" << endl;
            return;
        }
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        if ((result.contains("status") && result["status"].toString() != "ok")
            || (result.contains("err-code") && result["err-code"].toInt() != 0)) {
            QString errorMsg = result["err-msg"].toString();
            qDebug() << "[Sub][%d] Error: " << errorMsg << endl;
            m_webSocket.close();
        } else if (result.contains("op")) {
            QString op = result["op"].toString();
            if (op == "notify") {
                emit received(jsonObject);
            } else if (op == "ping") {
                sendPongOnTradingLine(jsonObject);
            } else if (op == "auth") {
                qDebug()<<"返回校验数据"<< endl;
                m_isNeedAuth = false;
                sendTopic();
            }
        } else if (result.contains("ch")) {
            emit received(jsonObject);
        } else if (result.contains("ping")) {
            sendPongOnMarketLine(jsonObject);
        } else if (result.contains("subbed")) {
            //订阅成功的返回信息，不处理
            qDebug() << "订阅" << result["subbed"].toString() << endl;
        }
    }
}
