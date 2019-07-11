#ifndef QTCHANNEL_H
#define QTCHANNEL_H

#include "Huobi/Enums.h"
#include "TimeService.h"
#include "QtJsonTool.h"
#include <QtCore/QObject>
#include <iostream>
#include "ApiSignature.h"

using namespace std;

namespace Huobi {

    class QtChannel : public QObject {
    Q_OBJECT
    public:

        static QString authChannel(string host, string accessKey, string secretKey) {
            time_t t = time(NULL);
            struct tm *local = gmtime(&t);
            char timeBuf[100] = {0};
            sprintf(timeBuf, "%04d-%02d-%02dT%02d%%3A%02d%%3A%02d",
                    local->tm_year + 1900,
                    local->tm_mon + 1,
                    local->tm_mday,
                    local->tm_hour,
                    local->tm_min,
                    local->tm_sec);
            char buf[100] = {0};
            sprintf(buf, "%04d-%02d-%02dT%02d:%02d:%02d",
                    local->tm_year + 1900,
                    local->tm_mon + 1,
                    local->tm_mday,
                    local->tm_hour,
                    local->tm_min,
                    local->tm_sec);
            string signa = ApiSignature::CreateSignature(host,
                                                              accessKey,
                                                              secretKey,
                                                              "/ws/v1", "GET", timeBuf, "");

            QJsonObject mjson;
            mjson.insert("SignatureVersion", "2");
            mjson.insert("op", "auth");
            mjson.insert("AccessKeyId", QString(accessKey.c_str()));
            mjson.insert("Signature", QString(signa.c_str()));
            mjson.insert("SignatureMethod", "HmacSHA256");
            mjson.insert("Timestamp", buf);

            QtJsonTool jsonTool;
            QString topic = jsonTool.getStringFromJsonObject(mjson);
            qDebug() << topic << endl;
            return topic;
        }


        static QList<QString> candlestickChannel(const char *symbols, const CandlestickInterval &interval) {
            QStringList symbollist = QString(symbols).split(',');
            QtJsonTool jsonTool;
            QList<QString> topics;
                    foreach(QString symbol, symbollist) {
                    QJsonObject mjson;
                    mjson.insert("sub", "market." + symbol + ".kline." + QString(interval.getValue().c_str()));
                    mjson.insert("id", QString(to_string(TimeService::getCurrentTimeStamp()).c_str()));
                    QString topic = jsonTool.getStringFromJsonObject(mjson);
                    qDebug() << topic << endl;
                    topics.append(topic);
                }

            return topics;
        }

        static QList<QString> tradeChannel(const char *symbols) {
            QStringList symbollist = QString(symbols).split(',');
            QtJsonTool jsonTool;
            QList<QString> topics;
                    foreach(QString symbol, symbollist) {
                    QJsonObject mjson;
                    mjson.insert("sub", "market." + symbol + ".trade.detail");
                    mjson.insert("id", QString(to_string(TimeService::getCurrentTimeStamp()).c_str()));
                    QString topic = jsonTool.getStringFromJsonObject(mjson);
                    qDebug() << topic << endl;
                    topics.append(topic);
                }

            return topics;
        }

        static QList<QString> priceDepthChannel(const char *symbols) {
            QStringList symbollist = QString(symbols).split(',');
            QtJsonTool jsonTool;

            QList<QString> topics;
                    foreach(QString symbol, symbollist) {
                    QJsonObject mjson;
                    mjson.insert("sub", "market." + symbol + ".depth.step0");
                    mjson.insert("id", QString(to_string(TimeService::getCurrentTimeStamp()).c_str()));
                    QString topic = jsonTool.getStringFromJsonObject(mjson);
                    qDebug() << topic << endl;
                    topics.append(topic);
                }

            return topics;
        }

        static QList<QString> accountChannel(const BalanceMode &mode) {
            QtJsonTool jsonTool;

            QList<QString> topics;

            QJsonObject mjson;
            mjson.insert("op", "sub");
            mjson.insert("cid", QString(to_string(TimeService::getCurrentTimeStamp()).c_str()));
            mjson.insert("topic", "accounts");
            mjson.insert("model", QString(mode.getValue().c_str()));

            QString topic = jsonTool.getStringFromJsonObject(mjson);
            qDebug() << topic << endl;
            topics.append(topic);

            return topics;
        }

        static QList<QString> orderUpdateChannel(const char *symbols) {
            QStringList symbollist = QString(symbols).split(',');
            QtJsonTool jsonTool;

            QList<QString> topics;
                    foreach(QString symbol, symbollist) {
                    QJsonObject mjson;
                    mjson.insert("op", "sub");
                    mjson.insert("cid", QString(to_string(TimeService::getCurrentTimeStamp()).c_str()));
                    mjson.insert("topic", "orders." + symbol);
                    QString topic = jsonTool.getStringFromJsonObject(mjson);
                    qDebug() << topic << endl;
                    topics.append(topic);
                }

            return topics;
        }

        static QList<QString> tradeStatisticsChannel(const char *symbols) {
            QStringList symbollist = QString(symbols).split(',');
            QtJsonTool jsonTool;

            QList<QString> topics;
                    foreach(QString symbol, symbollist) {
                    QJsonObject mjson;
                    mjson.insert("sub", "market." + symbol + ".detail");
                    mjson.insert("id", QString(to_string(TimeService::getCurrentTimeStamp()).c_str()));
                    QString topic = jsonTool.getStringFromJsonObject(mjson);
                    qDebug() << topic << endl;
                    topics.append(topic);
                }

            return topics;
        }
    };
}
#endif /* QTCHANNEL_H */

