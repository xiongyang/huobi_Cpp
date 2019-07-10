#include "QtSubscribeClientImpl.h"
#include <iostream>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <Huobi/Decimal.h>


using namespace std;

QT_USE_NAMESPACE

namespace Huobi {
    QtSubscribeClientImpl::QtSubscribeClientImpl(const char *accessKey, const char *secretKey, const string &url,
                                                 QObject *parent) :
            QObject(parent) {
        m_accessKey = accessKey;
        m_secretKey = secretKey;
        m_url = url;

    }

    QtSubscribeClientImpl::QtSubscribeClientImpl(const char *accessKey, const char *secretKey, QObject *parent) :
            QObject(parent) {
        m_accessKey = accessKey;
        m_secretKey = secretKey;
    }

    QtSubscribeClientImpl::QtSubscribeClientImpl(const string &url, QObject *parent) :
            QObject(parent) {
        m_url = url;
    }

    QtSubscribeClientImpl::QtSubscribeClientImpl(QObject *parent) :
            QObject(parent) {

//        impl = new WebSocketApiImpl(apiKey, secretKey);
//        if (!op.url.empty()) {
//            host = GetHost(op.url);
//            RequestOptions resop;
//            resop.url = op.url;
//            RestApiImpl* restimpl = new RestApiImpl(apiKey.c_str(), secretKey.c_str(), resop);
//            AccountsInfoMap::updateUserInfo(apiKey, restimpl);
//            delete restimpl;
//        } else {
//            RestApiImpl* restimpl = new RestApiImpl(apiKey.c_str(), secretKey.c_str());
//            AccountsInfoMap::updateUserInfo(apiKey, restimpl);
//            delete restimpl;
//        }
    }

    QtSubscribeClientImpl::~QtSubscribeClientImpl() {
        qDeleteAll(m_pClientList);
        m_pClientList.clear();
    }

    void QtSubscribeClientImpl::subscribeCandlestickEvent(
            const char *symbols,
            CandlestickInterval interval,
            const std::function<void(const CandlestickEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::candlestickChannel(symbols, interval);

        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedCandlestickEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalCandlestickEvent, this, callback);

        QList<QtWebSocketClient *> wsclist;
        wsclist.append(pClient);
        m_pClientList.append(wsclist);
    }

    void QtSubscribeClientImpl::subscribeTradeEvent(
            const char *symbols,
            const std::function<void(const TradeEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::tradeChannel(symbols);

        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedTradeEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalTradeEvent, this, callback);

        QList<QtWebSocketClient *> wsclist;
        wsclist.append(pClient);
        m_pClientList.append(wsclist);
    }


    void QtSubscribeClientImpl::subscribePriceDepthEvent(
            const char *symbols,
            const std::function<void(const PriceDepthEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::priceDepthChannel(symbols);

        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedPriceDepthEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalPriceDepthEvent, this, callback);

        QList<QtWebSocketClient *> wsclist;
        wsclist.append(pClient);
        m_pClientList.append(wsclist);
    }

    void QtSubscribeClientImpl::subscribe24HTradeStatisticsEvent(
            const char *symbols,
            const std::function<void(const TradeStatisticsEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::tradeStatisticsChannel(symbols);

        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedTradeStatisticsEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalTradeStatisticsEvent, this, callback);

        QList<QtWebSocketClient *> wsclist;
        wsclist.append(pClient);
        m_pClientList.append(wsclist);
    }

    void QtSubscribeClientImpl::subscribeOrderUpdateEvent(
            const char *symbols,
            const std::function<void(const OrderUpdateEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::orderUpdateChannel(symbols);

        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedOrderUpdateEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalOrderUpdateEvent, this, callback);

        QList<QtWebSocketClient *> wsclist;
        wsclist.append(pClient);
        m_pClientList.append(wsclist);
    }

    void QtSubscribeClientImpl::subscribeAccountEvent(
            const BalanceMode &mode,
            const std::function<void(const AccountEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::accountChannel(mode);

        auto *pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedAccountEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalAccountEvent, this, callback);

        QList<QtWebSocketClient *> wsclist;
        wsclist.append(pClient);
        m_pClientList.append(wsclist);
    }

    void QtSubscribeClientImpl::onReceivedCandlestickEvent(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        CandlestickEvent event;

        QString ch = result["ch"].toString();

        event.symbol = ch.split('.').at(1).toStdString();
        event.interval = CandlestickInterval::lookup(ch.split('.').at(3).toStdString());
        event.timestamp = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());

        QVariantMap tick = result["tick"].toMap();

        Candlestick data;
        data.timestamp = TimeService::convertCSTInSecondToUTC(tick["id"].toULongLong());
        data.amount = toDecimal(tick["amount"]);
        data.close = toDecimal(tick["close"]);
        data.high = toDecimal(tick["high"]);
        data.low = toDecimal(tick["low"]);
        data.open = toDecimal(tick["open"]);
        data.volume = toDecimal(tick["vol"]);
        data.count = tick["count"].toULongLong();
        event.data = data;

        cout << "发送信号" << endl;
        emit callbackSignalCandlestickEvent(event);
    }

    void QtSubscribeClientImpl::onReceivedTradeEvent(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        TradeEvent event;

        QString ch = result["ch"].toString();
        event.symbol = ch.split('.').at(1).toStdString();
        event.timestamp = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());

        QVariantMap tick = result["tick"].toMap();
                foreach (QVariant data, tick["data"].toList()) {
                QVariantMap item = data.toMap();
                Trade trade;
                trade.amount = toDecimal(item["amount"]);
                trade.price = toDecimal(item["price"]);
                trade.tradeId = item["id"].toString().toStdString();
                trade.direction = TradeDirection::lookup(item["direction"].toString().toStdString());
                trade.timestamp = TimeService::convertCSTInMillisecondToUTC(item["ts"].toULongLong());
                event.tradeList.push_back(trade);
            }

        cout << "发送信号" << endl;
        emit callbackSignalTradeEvent(event);
    }


    void QtSubscribeClientImpl::onReceivedPriceDepthEvent(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        PriceDepthEvent event;
        QString ch = result["ch"].toString();
        event.symbol = ch.split('.').at(1).toStdString();
        event.timestamp = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());

        PriceDepth depth;
        std::vector<DepthEntry> bidsves;
        std::vector<DepthEntry> asksves;
        QVariantMap tick = result["tick"].toMap();
                foreach (QVariant bid, tick["bids"].toList()) {
                DepthEntry de;
                de.price = toDecimal(bid.toList().at(0));
                de.amount = toDecimal(bid.toList().at(1));
                bidsves.push_back(de);
            }
                foreach (QVariant ask, tick["asks"].toList()) {
                DepthEntry de;
                de.price = toDecimal(ask.toList().at(0));
                de.amount = toDecimal(ask.toList().at(1));
                asksves.push_back(de);
            }


        depth.bids = bidsves;
        depth.asks = asksves;

        event.data = depth;

        cout << "发送信号" << endl;
        emit callbackSignalPriceDepthEvent(event);
    }

    void QtSubscribeClientImpl::onReceivedTradeStatisticsEvent(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        TradeStatisticsEvent event;

        QString ch = result["ch"].toString();

        event.symbol = ch.split('.').at(1).toStdString();
        long ts = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());
        event.timestamp = ts;
        QVariantMap tick = result["tick"].toMap();

        TradeStatistics statistics;
        statistics.amount = toDecimal(tick["amount"]);
        statistics.open = toDecimal(tick["open"]);
        statistics.close = toDecimal(tick["close"]);
        statistics.high = toDecimal(tick["high"]);
        statistics.timestamp = ts;
        statistics.count = tick["count"].toULongLong();
        statistics.low = toDecimal(tick["low"]);
        statistics.volume = toDecimal(tick["vol"]);
        event.tradeStatistics = statistics;

        cout << "发送信号" << endl;
        emit callbackSignalTradeStatisticsEvent(event);
    }

    void QtSubscribeClientImpl::onReceivedOrderUpdateEvent(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        OrderUpdateEvent event;

        QString topic = result["topic"].toString();

        event.symbol = topic.split('.').at(1).toStdString();
        event.timestamp = TimeService::convertCSTInMillisecondToUTC(result["ts"].toULongLong());
        QVariantMap data = result["data"].toMap();

        Order order;
        order.orderId = data["order-id"].toULongLong();
        order.symbol = data["symbol"].toString().toStdString();
        order.accountType = AccountsInfoMap::getAccount(m_accessKey, data["account-id"].toULongLong()).type;
        order.amount = toDecimal(data["order-amount"]);
        order.price = toDecimal(data["order-price"]);
        order.createdTimestamp = TimeService::convertCSTInMillisecondToUTC(data["created-at"].toULongLong());
        order.type = OrderType::lookup(data["order-type"].toString().toStdString());
        order.filledAmount = toDecimal(data["filled-amount"]);
        order.filledCashAmount = toDecimal(data["filled-cash-amount"]);
        order.filledFees = toDecimal(data["filled-fees"]);
        order.state = OrderState::lookup(data["order-state"].toString().toStdString());
        order.source = OrderSource::lookup(data["order-source"].toString().toStdString());
        event.data = order;

        cout << "发送信号" << endl;
        emit callbackSignalOrderUpdateEvent(event);
    }

    void QtSubscribeClientImpl::onReceivedAccountEvent(QJsonObject jsonObject) {
        cout << "收到信号" << endl;
        QJsonDocument jsonDocument(jsonObject);
        QVariantMap result = jsonDocument.toVariant().toMap();

        AccountEvent event;

        QVariantMap data = result["data"].toMap();
        event.changeType = AccountChangeType::lookup(data["event"].toString().toStdString());
        event.timestamp = TimeService::convertCSTInMillisecondToUTC(data["ts"].toULongLong());
                foreach (QVariant l, data["list"].toList()) {
                QVariantMap item = l.toMap();

                AccountChange change;
                change.accountType = AccountsInfoMap::getAccount(m_accessKey, item["account-id"].toULongLong()).type;
                change.currency = item["currency"].toString().toStdString();
                change.balance = toDecimal(item["balance"]);
                change.balanceType = BalanceType::lookup(item["type"].toString().toStdString());
                event.accountChangeList.push_back(change);
            }


        cout << "发送信号" << endl;
        emit callbackSignalAccountEvent(event);
    }

    Decimal QtSubscribeClientImpl::toDecimal(QVariant num) {
        return Decimal(num.toString().toStdString().c_str());
    }
}






