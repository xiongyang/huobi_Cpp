#ifndef QTCHANNEL_H
#define QTCHANNEL_H

#include "Huobi/Enums.h"
#include "TimeService.h"
#include "QtJsonTool.h"
#include <QtCore/QObject>
#include <iostream>

using namespace std;

namespace Huobi {

    class QtChannel : public QObject {
    Q_OBJECT
    public:

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

