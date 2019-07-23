#ifndef QTRESTCLIENTIMPL_H
#define QTRESTCLIENTIMPL_H

#include <QtCore/QObject>
#include <iostream>
#include <QtCore/QUrl>
#include <string>
#include <functional>
#include "Huobi/HuobiApiException.h"
#include "Huobi/PriceDepthEvent.h"
#include "Huobi/QtRestClient.h"
#include <QList>
#include <QtCore/QJsonDocument>
#include <Huobi/Candlestick.h>
#include <Huobi/CandlestickRequest.h>
#include "QtWebSocketClient.h"
#include "Utils/QtJsonTool.h"
#include "Utils/QtChannel.h"
#include "AccountsInfoMap.h"
#include "QtHttpClient.h"
#include "InputChecker.h"
#include <QUrlQuery>


using namespace std;
using namespace Huobi;

QT_FORWARD_DECLARE_CLASS(QtWebSocketClient)

namespace Huobi {
    class QtRestClientImpl : public QObject, public RestClient {


    Q_OBJECT
    public:

        explicit QtRestClientImpl(QObject *parent = nullptr);

        explicit QtRestClientImpl(const char *accessKey, const char *secretKey, QObject *parent = nullptr);


        ~QtRestClientImpl();

    public:
//        std::vector<Candlestick> getCandlestick(const CandlestickRequest &request) override;

        void getLatestCandlestick(const std::string &symbol,
                                  const CandlestickInterval &interval,
                                  int size,
                                  long startTime,
                                  long endTime,
                                  const std::function<void(vector<Candlestick>)> &callback,
                                  const std::function<void(
                                          HuobiApiException &)> &errorHandler = std::function<void(
                                          HuobiApiException &)>()) override;

        void getHistoricalTrade(const char* symbol, int size,
                                        const std::function<void(vector<Trade>)> &callback,
                                        const std::function<void(
                                                HuobiApiException & )> &errorHandler = std::function<void(
                                                HuobiApiException & )>()) override;


//        void subscribeTradeEvent(
//                const char *symbols,
//                const std::function<void(const TradeEvent &)> &callback,
//                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
//                        HuobiApiException &)>()) override;
//
        void getPriceDepth(
                const char *symbol, int size,
                const std::function<void(PriceDepth)> &callback,
                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
                        HuobiApiException &)>()) override;

//        void subscribe24HTradeStatisticsEvent(
//                const char *symbols,
//                const std::function<void(const TradeStatisticsEvent &)> &callback,
//                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
//                        HuobiApiException &)>()) override;
//
//        void subscribeOrderUpdateEvent(
//                const char *symbols,
//                const std::function<void(const OrderUpdateEvent &)> &callback,
//                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
//                        HuobiApiException &)>()) override;
//
//        void subscribeAccountEvent(
//                const BalanceMode &mode,
//                const std::function<void(const AccountEvent &)> &callback,
//                const std::function<void(HuobiApiException &)> &errorHandler = std::function<void(
//                        HuobiApiException &)>()) override;

    private Q_SLOTS:

        void onReceivedCandlestick(QJsonObject jsonObject);
        void onReceivedHistoricalTrade(QJsonObject jsonObject);


        //
//        void onReceivedTradeEvent(QJsonObject jsonObject);
//
        void onReceivedPriceDepth(QJsonObject jsonObject);
//
//        void onReceivedTradeStatisticsEvent(QJsonObject jsonObject);
//
//        void onReceivedOrderUpdateEvent(QJsonObject jsonObject);
//
//        void onReceivedAccountEvent(QJsonObject jsonObject);


    signals:

        void callbackSignalCandlestick(vector<Candlestick> event);
        void callbackSignalHistoricalTrade(vector<Trade> event);


        //
//        void callbackSignalTradeEvent(TradeEvent event);
//
        void callbackSignalPriceDepth(PriceDepth event);
//
//        void callbackSignalTradeStatisticsEvent(TradeStatisticsEvent event);
//
//        void callbackSignalOrderUpdateEvent(OrderUpdateEvent event);
//
//        void callbackSignalAccountEvent(AccountEvent event);

    private:
        string m_accessKey;
        string m_secretKey;
        string m_url;
        string m_host = "api.huobi.pro";
        string m_port = "443";
        string m_tradeUrl = "https://" + m_host + ":" + m_port;
        string m_marketUrl = "https://" + m_host + ":" + m_port;

        QList<QtHttpClient *> m_pHttpClientList;
    };
}


#endif /* QTRESTCLIENTIMPL_H */

