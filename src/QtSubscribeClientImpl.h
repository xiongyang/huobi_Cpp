#ifndef QTSUBSCRIBECLIENTIMPL_H
#define QTSUBSCRIBECLIENTIMPL_H

#include <QtCore/QObject>
#include <iostream>
#include <QtCore/QUrl>
#include <string>
#include <functional>
#include "Huobi/HuobiApiException.h"
#include "Huobi/PriceDepthEvent.h"
#include "Huobi/QtSubscribeClient.h"
#include <QList>
#include <QtCore/QJsonDocument>
#include "QtWebSocketClient.h"
#include "Utils/QtJsonTool.h"
#include "Utils/QtChannel.h"
#include "AccountsInfoMap.h"


using namespace std;

QT_FORWARD_DECLARE_CLASS(QtWebSocketClient)

namespace Huobi {
    class QtSubscribeClientImpl : public QObject, public SubscribeClient {


    Q_OBJECT
    public:

        explicit QtSubscribeClientImpl(QObject *parent = nullptr);

        explicit QtSubscribeClientImpl(const char *accessKey, const char *secretKey, QObject *parent = nullptr);


        ~QtSubscribeClientImpl();

    public:
        void subscribeCandlestickEvent(
                const char *symbols,
                CandlestickInterval interval,
                const std::function<void(const CandlestickEvent &)> &callback,
                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
                        HuobiApiException &)>()) override;

        void subscribeTradeEvent(
                const char *symbols,
                const std::function<void(const TradeEvent &)> &callback,
                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
                        HuobiApiException &)>()) override;

        void subscribePriceDepthEvent(
                const char *symbols,
                const std::function<void(const PriceDepthEvent &)> &callback,
                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
                        HuobiApiException &)>()) override;

        void subscribe24HTradeStatisticsEvent(
                const char *symbols,
                const std::function<void(const TradeStatisticsEvent &)> &callback,
                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
                        HuobiApiException &)>()) override;

        void subscribeOrderUpdateEvent(
                const char *symbols,
                const std::function<void(const OrderUpdateEvent &)> &callback,
                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
                        HuobiApiException &)>()) override;

        void subscribeAccountEvent(
                const BalanceMode &mode,
                const std::function<void(const AccountEvent &)> &callback,
                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
                        HuobiApiException &)>()) override;

    private Q_SLOTS:

        void onReceivedCandlestickEvent(QJsonObject jsonObject);

        void onReceivedTradeEvent(QJsonObject jsonObject);

        void onReceivedPriceDepthEvent(QJsonObject jsonObject);

        void onReceivedTradeStatisticsEvent(QJsonObject jsonObject);

        void onReceivedOrderUpdateEvent(QJsonObject jsonObject);

        void onReceivedAccountEvent(QJsonObject jsonObject);


    signals:

        void callbackSignalCandlestickEvent(CandlestickEvent event);

        void callbackSignalTradeEvent(TradeEvent event);

        void callbackSignalPriceDepthEvent(PriceDepthEvent event);

        void callbackSignalTradeStatisticsEvent(TradeStatisticsEvent event);

        void callbackSignalOrderUpdateEvent(OrderUpdateEvent event);

        void callbackSignalAccountEvent(AccountEvent event);

    private:
        string m_accessKey;
        string m_secretKey;
        string m_url;
        string m_host = "api.huobi.pro";
        string m_port = "443";
        string m_marketUrl = "wss://" + m_host + ":" + m_port + "/ws";
        string m_tradeUrl = "wss://" + m_host + ":" + m_port + "/ws/v1";
        QList<QtWebSocketClient *> m_pClientList;
    };
}


#endif /* QTSUBSCRIBECLIENTIMPL_H */

