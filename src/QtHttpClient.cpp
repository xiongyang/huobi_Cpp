#include "QtHttpClient.h"
#include <QUrl>
#include <QByteArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>
#include <QTextCodec>

using namespace std;
using namespace Huobi;

QT_USE_NAMESPACE

namespace Huobi {
    QtHttpClient::QtHttpClient(QObject *parent) :
            QObject(parent) {
        QObject::connect(&m_pManager, &QNetworkAccessManager::finished, this, &QtHttpClient::replyFinished);
    }


    QtHttpClient::~QtHttpClient() {

    }

    void QtHttpClient::get(QUrl url) {
        QNetworkRequest request;
        request.setUrl(url);
        m_pManager.get(request);
    }

    bool QtHttpClient::post(QUrl url, QByteArray &bytes) {
        QNetworkRequest request;
        request.setUrl(url);
//        QNetworkReply *reply = m_pManager->post(request, bytes);
//        connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
//                this, SLOT(downloadProgress(qint64, qint64)));
//        return true;
    }

//    void QtHttpClient::finished() {
//        QNetworkReply *reply = (QNetworkReply *) sender();
//        reply->deleteLater();
//    }




    void QtHttpClient::replyFinished(QNetworkReply *reply) {
        QJsonObject jsonObject = QtJsonTool::getJsonObjectFromByteArray(reply->readAll());
//        qDebug() << "收到数据~~~~~~~~~~~~" << jsonObject << endl;
        emit received(jsonObject);
//        cout<< "收到数据"<< endl;
//        QTextCodec *codec = QTextCodec::codecForName("utf8");

        //使用utf8编码，这样才可以显示中文
//        QString all = codec->toUnicode(reply->readAll());
//        cout << all.toStdString() << endl;
        reply->deleteLater();
    }
}
