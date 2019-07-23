#include "QtRestClientImpl.h"
#include <iostream>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include "Utils/QtDecimalTool.h"


using namespace std;
using namespace Huobi;

QT_USE_NAMESPACE

namespace Huobi {

    QtRestClientImpl::QtRestClientImpl(const char *accessKey, const char *secretKey, QObject *parent) :
            QObject(parent) {
        m_accessKey = accessKey;
        m_secretKey = secretKey;
    }


    QtRestClientImpl::QtRestClientImpl(QObject *parent) :
            QObject(parent) {
    }

    QtRestClientImpl::~QtRestClientImpl() {
        qDeleteAll(m_pHttpClientList);
        m_pHttpClientList.clear();
    }

//    std::vector<Candlestick> QtRestClientImpl::getCandlestick(const CandlestickRequest &request) {
//        QList<QString> topics = QtChannel::candlestickChannel(symbols, interval);
//        m_url = m_marketUrl;

//        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
//        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedCandlestickEvent);
//        connect(this, &QtSubscribeClientImpl::callbackSignalCandlestickEvent, this, callback);

//        QList<QtWebSocketClient *> wsclist;
//        wsclist.append(pClient);
//        m_pClientList.append(wsclist);
//    }

    void QtRestClientImpl::getLatestCandlestick(const std::string &symbol,
                                                const CandlestickInterval &interval,
                                                int size,
                                                long startTime,
                                                long endTime,
                                                const std::function<void(vector<Candlestick>)> &callback,
                                                const std::function<void(
                                                        HuobiApiException &)> &errorHandler) {
        InputChecker::checker()
                ->checkSymbol(symbol)
                ->checkRange(size, 1, 2000, "size");

        m_url = m_marketUrl;
        QString path = "/market/history/kline";
        QUrl url(QString::fromStdString(m_url) + path);

        QUrlQuery query;
        query.addQueryItem("symbol", QString::fromStdString(symbol));
        query.addQueryItem("period", QString::fromStdString(interval.getValue()));
        query.addQueryItem("size", QString::number(size));
        query.addQueryItem("start", QString::number(startTime));
        query.addQueryItem("end", QString::number(endTime));
        url.setQuery(query);

        qDebug() << url;

        auto pHttpClient = new QtHttpClient();
        connect(pHttpClient, &QtHttpClient::received, this, &QtRestClientImpl::onReceivedCandlestick);
        connect(this, &QtRestClientImpl::callbackSignalCandlestick, this, callback);

        pHttpClient->get(url);
        m_pHttpClientList.append(pHttpClient);
    }

    void QtRestClientImpl::getHistoricalTrade(const char *symbol, int size,
                                              const std::function<void(vector<Trade>)> &callback,
                                              const std::function<void(
                                                      HuobiApiException &)> &errorHandler) {
        InputChecker::checker()
                ->checkSymbol(symbol)
                ->checkRange(size, 1, 2000, "size");

        m_url = m_marketUrl;
        QString path = "/market/history/trade";
        QUrl url(QString::fromStdString(m_url) + path);

        QUrlQuery query;
        query.addQueryItem("symbol", QString::fromStdString(symbol));
        query.addQueryItem("size", QString::number(size));
        url.setQuery(query);

        qDebug() << url;

        auto pHttpClient = new QtHttpClient();
        connect(pHttpClient, &QtHttpClient::received, this, &QtRestClientImpl::onReceivedHistoricalTrade);
        connect(this, &QtRestClientImpl::callbackSignalHistoricalTrade, this, callback);

        pHttpClient->get(url);
        m_pHttpClientList.append(pHttpClient);
    }

    void QtRestClientImpl::onReceivedCandlestick(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        vector<Candlestick> event;

                foreach (QVariant data, result["data"].toList()) {
                QVariantMap item = data.toMap();
                Candlestick candlestick;
                candlestick.timestamp = TimeService::convertCSTInSecondToUTC(item["id"].toULongLong());
                candlestick.amount = QtDecimalTool::toDecimal(item["amount"]);
                candlestick.close = QtDecimalTool::toDecimal(item["close"]);
                candlestick.high = QtDecimalTool::toDecimal(item["high"]);
                candlestick.low = QtDecimalTool::toDecimal(item["low"]);
                candlestick.open = QtDecimalTool::toDecimal(item["open"]);
                candlestick.volume = QtDecimalTool::toDecimal(item["vol"]);
                candlestick.count = item["count"].toULongLong();

                event.push_back(candlestick);
            }


        cout << "发送信号" << endl;
        emit callbackSignalCandlestick(event);
    }

    void QtRestClientImpl::onReceivedHistoricalTrade(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        vector<Trade> event;

                foreach (QVariant data, result["data"].toList()) {
                QVariantMap item = data.toMap();
                        foreach (QVariant dataIn, item["data"].toList()) {
                        QVariantMap itemIn = dataIn.toMap();
                        Trade trade;
                        trade.price = QtDecimalTool::toDecimal(itemIn["price"]);
                        trade.amount = QtDecimalTool::toDecimal(itemIn["amount"]);
                        trade.tradeId = itemIn["id"].toString().toStdString();
                        trade.timestamp = TimeService::convertCSTInSecondToUTC(itemIn["ts"].toULongLong());
                        trade.direction = TradeDirection::lookup(itemIn["direction"].toString().toStdString());

                        event.push_back(trade);
                    }
            }


        cout << "发送信号" << endl;
        emit callbackSignalHistoricalTrade(event);
    }


//    void QtSubscribeClientImpl::subscribeTradeEvent(
//            const char *symbols,
//            const std::function<void(const TradeEvent &)> &callback,
//            const std::function<void(HuobiApiException &)> &errorHandler) {
//        QList<QString> topics = QtChannel::tradeChannel(symbols);
//        m_url = m_marketUrl;
//        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
//        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedTradeEvent);
//        connect(this, &QtSubscribeClientImpl::callbackSignalTradeEvent, this, callback);
//
//        QList<QtWebSocketClient *> wsclist;
//        wsclist.append(pClient);
//        m_pClientList.append(wsclist);
//    }


    void QtRestClientImpl::getPriceDepth(
            const char *symbol, int size,
            const std::function<void(PriceDepth)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        InputChecker::checker()->checkSymbol(symbol)->checkRange(size, 1, 150, "size");

        m_url = m_marketUrl;

        QString path = "/market/depth";
        QUrl url(QString::fromStdString(m_url) + path);

        QUrlQuery query;
        query.addQueryItem("symbol", QString::fromStdString(symbol));
        query.addQueryItem("type", "step0");

        url.setQuery(query);

        qDebug() << url;

        auto pHttpClient = new QtHttpClient();
        connect(pHttpClient, &QtHttpClient::received, this, &QtRestClientImpl::onReceivedPriceDepth);
        connect(this, &QtRestClientImpl::callbackSignalPriceDepth, this, callback);

        pHttpClient->get(url);
        m_pHttpClientList.append(pHttpClient);
    }

//    void QtSubscribeClientImpl::subscribe24HTradeStatisticsEvent(
//            const char *symbols,
//            const std::function<void(const TradeStatisticsEvent &)> &callback,
//            const std::function<void(HuobiApiException &)> &errorHandler) {
//        QList<QString> topics = QtChannel::tradeStatisticsChannel(symbols);
//        m_url = m_marketUrl;
//
//        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
//        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedTradeStatisticsEvent);
//        connect(this, &QtSubscribeClientImpl::callbackSignalTradeStatisticsEvent, this, callback);
//
//        QList<QtWebSocketClient *> wsclist;
//        wsclist.append(pClient);
//        m_pClientList.append(wsclist);
//    }

//    void QtSubscribeClientImpl::subscribeOrderUpdateEvent(
//            const char *symbols,
//            const std::function<void(const OrderUpdateEvent &)> &callback,
//            const std::function<void(HuobiApiException &)> &errorHandler) {
//        QList<QString> topics = QtChannel::orderUpdateChannel(symbols);
//        QString authTopic = QtChannel::authChannel(m_host, m_accessKey, m_secretKey);
//        m_url = m_tradeUrl;
//        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics,authTopic);
//        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedOrderUpdateEvent);
//        connect(this, &QtSubscribeClientImpl::callbackSignalOrderUpdateEvent, this, callback);
//
//        QList<QtWebSocketClient *> wsclist;
//        wsclist.append(pClient);
//        m_pClientList.append(wsclist);
//    }

//    void QtSubscribeClientImpl::subscribeAccountEvent(
//            const BalanceMode &mode,
//            const std::function<void(const AccountEvent &)> &callback,
//            const std::function<void(HuobiApiException &)> &errorHandler) {
//        QList<QString> topics = QtChannel::accountChannel(mode);
//        QString authTopic = QtChannel::authChannel(m_host, m_accessKey, m_secretKey);
//        m_url = m_tradeUrl;
//        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics, authTopic);
//        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedAccountEvent);
//        connect(this, &QtSubscribeClientImpl::callbackSignalAccountEvent, this, callback);
//
//        QList<QtWebSocketClient *> wsclist;
//        wsclist.append(pClient);
//        m_pClientList.append(wsclist);
//    }

//    void QtSubscribeClientImpl::onReceivedCandlestickEvent(QJsonObject jsonObject) {
//        cout << "收到信号" << endl;
//        QJsonDocument jsonDocument(jsonObject);
//        QVariantMap result = jsonDocument.toVariant().toMap();
//
//        CandlestickEvent event;
//
//        QString ch = result["ch"].toString();
//
//        event.symbol = ch.split('.').at(1).toStdString();
//        event.interval = CandlestickInterval::lookup(ch.split('.').at(3).toStdString());
//        event.timestamp = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());
//
//        QVariantMap tick = result["tick"].toMap();
//
//        Candlestick data;
//        data.timestamp = TimeService::convertCSTInSecondToUTC(tick["id"].toULongLong());
//        data.amount = QtDecimalTool::toDecimal(tick["amount"]);
//        data.close = QtDecimalTool::toDecimal(tick["close"]);
//        data.high = QtDecimalTool::toDecimal(tick["high"]);
//        data.low = QtDecimalTool::toDecimal(tick["low"]);
//        data.open = QtDecimalTool::toDecimal(tick["open"]);
//        data.volume = QtDecimalTool::toDecimal(tick["vol"]);
//        data.count = tick["count"].toULongLong();
//        event.data = data;
//
//        cout << "发送信号" << endl;
//        emit callbackSignalCandlestickEvent(event);
//    }

//    void QtSubscribeClientImpl::onReceivedTradeEvent(QJsonObject jsonObject) {
//        cout << "收到信号" << endl;
//        QJsonDocument jsonDocument(jsonObject);
//        QVariantMap result = jsonDocument.toVariant().toMap();
//
//        TradeEvent event;
//
//        QString ch = result["ch"].toString();
//        event.symbol = ch.split('.').at(1).toStdString();
//        event.timestamp = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());
//
//        QVariantMap tick = result["tick"].toMap();
//                foreach (QVariant data, tick["data"].toList()) {
//                QVariantMap item = data.toMap();
//                Trade trade;
//                trade.amount = QtDecimalTool::toDecimal(item["amount"]);
//                trade.price = QtDecimalTool::toDecimal(item["price"]);
//                trade.tradeId = item["id"].toString().toStdString();
//                trade.direction = TradeDirection::lookup(item["direction"].toString().toStdString());
//                trade.timestamp = TimeService::convertCSTInMillisecondToUTC(item["ts"].toULongLong());
//                event.tradeList.push_back(trade);
//            }
//
//        cout << "发送信号" << endl;
//        emit callbackSignalTradeEvent(event);
//    }


    void QtRestClientImpl::onReceivedPriceDepth(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        PriceDepth event;
        std::vector<DepthEntry> bidsves;
        std::vector<DepthEntry> asksves;

        QVariantMap tick = result["tick"].toMap();
        event.timestamp = TimeService::convertCSTInMillisecondToUTC(tick["ts"].toULongLong());

                foreach (QVariant bid, tick["bids"].toList()) {
                DepthEntry de;
                de.price = QtDecimalTool::toDecimal(bid.toList().at(0));
                de.amount = QtDecimalTool::toDecimal(bid.toList().at(1));
                bidsves.push_back(de);
            }
                foreach (QVariant ask, tick["asks"].toList()) {
                DepthEntry de;
                de.price = QtDecimalTool::toDecimal(ask.toList().at(0));
                de.amount = QtDecimalTool::toDecimal(ask.toList().at(1));
                asksves.push_back(de);
            }

        event.bids = bidsves;
        event.asks = asksves;

        cout << "发送信号" << endl;
        emit callbackSignalPriceDepth(event);
    }

//    void QtSubscribeClientImpl::onReceivedTradeStatisticsEvent(QJsonObject jsonObject) {
//        cout << "收到信号" << endl;
//        QJsonDocument jsonDocument(jsonObject);
//        QVariantMap result = jsonDocument.toVariant().toMap();
//
//        TradeStatisticsEvent event;
//
//        QString ch = result["ch"].toString();
//
//        event.symbol = ch.split('.').at(1).toStdString();
//        long ts = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());
//        event.timestamp = ts;
//        QVariantMap tick = result["tick"].toMap();
//
//        TradeStatistics statistics;
//        statistics.amount = QtDecimalTool::toDecimal(tick["amount"]);
//        statistics.open = QtDecimalTool::toDecimal(tick["open"]);
//        statistics.close = QtDecimalTool::toDecimal(tick["close"]);
//        statistics.high = QtDecimalTool::toDecimal(tick["high"]);
//        statistics.timestamp = ts;
//        statistics.count = tick["count"].toULongLong();
//        statistics.low = QtDecimalTool::toDecimal(tick["low"]);
//        statistics.volume = QtDecimalTool::toDecimal(tick["vol"]);
//        event.tradeStatistics = statistics;
//
//        cout << "发送信号" << endl;
//        emit callbackSignalTradeStatisticsEvent(event);
//    }

//    void QtSubscribeClientImpl::onReceivedOrderUpdateEvent(QJsonObject jsonObject) {
//        cout << "收到信号" << endl;
//        QJsonDocument jsonDocument(jsonObject);
//        QVariantMap result = jsonDocument.toVariant().toMap();
//
//        OrderUpdateEvent event;
//
//        QString topic = result["topic"].toString();
//
//        event.symbol = topic.split('.').at(1).toStdString();
//        event.timestamp = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());
//        QVariantMap data = result["data"].toMap();
//
//        Order order;
//        order.orderId = data["order-id"].toULongLong();
//        order.symbol = data["symbol"].toString().toStdString();
//        order.accountType = AccountsInfoMap::getAccount(m_accessKey, data["account-id"].toULongLong()).type;
//        order.amount = QtDecimalTool::toDecimal(data["order-amount"]);
//        order.price = QtDecimalTool::toDecimal(data["order-price"]);
//        order.createdTimestamp = TimeService::convertCSTInMillisecondToUTC(data["created-at"].toULongLong());
//        order.type = OrderType::lookup(data["order-type"].toString().toStdString());
//        order.filledAmount = QtDecimalTool::toDecimal(data["filled-amount"]);
//        order.filledCashAmount = QtDecimalTool::toDecimal(data["filled-cash-amount"]);
//        order.filledFees = QtDecimalTool::toDecimal(data["filled-fees"]);
//        order.state = OrderState::lookup(data["order-state"].toString().toStdString());
//        order.source = OrderSource::lookup(data["order-source"].toString().toStdString());
//        event.data = order;
//
//        cout << "发送信号" << endl;
//        emit callbackSignalOrderUpdateEvent(event);
//    }

//    void QtSubscribeClientImpl::onReceivedAccountEvent(QJsonObject jsonObject) {
//        cout << "收到信号" << endl;
//        QJsonDocument jsonDocument(jsonObject);
//        QVariantMap result = jsonDocument.toVariant().toMap();
//
//        AccountEvent event;
//
//        QVariantMap data = result["data"].toMap();
//        event.changeType = AccountChangeType::lookup(data["event"].toString().toStdString());
//        event.timestamp = TimeService::convertCSTInMillisecondToUTC(data["ts"].toULongLong());
//                foreach (QVariant l, data["list"].toList()) {
//                QVariantMap item = l.toMap();
//
//                AccountChange change;
//                change.accountType = AccountsInfoMap::getAccount(m_accessKey, item["account-id"].toULongLong()).type;
//                change.currency = item["currency"].toString().toStdString();
//                change.balance = QtDecimalTool::toDecimal(item["balance"]);
//                change.balanceType = BalanceType::lookup(item["type"].toString().toStdString());
//                event.accountChangeList.push_back(change);
//            }
//
//
//        cout << "发送信号" << endl;
//        emit callbackSignalAccountEvent(event);
//    }
}






