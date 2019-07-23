#include "QtSubscribeClientImpl.h"
#include <iostream>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include "Utils/QtDecimalTool.h"


using namespace std;

QT_USE_NAMESPACE

namespace Huobi {

    QtSubscribeClientImpl::QtSubscribeClientImpl(const char *accessKey, const char *secretKey, QObject *parent) :
            QObject(parent) {
        m_accessKey = accessKey;
        m_secretKey = secretKey;
    }


    QtSubscribeClientImpl::QtSubscribeClientImpl(QObject *parent) :
            QObject(parent) {
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
        m_url = m_marketUrl;

        auto pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedCandlestickEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalCandlestickEvent, this, callback);
        m_pClientList.append(pClient);
    }

    void QtSubscribeClientImpl::subscribeTradeEvent(
            const char *symbols,
            const std::function<void(const TradeEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::tradeChannel(symbols);
        m_url = m_marketUrl;
        auto pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedTradeEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalTradeEvent, this, callback);

        m_pClientList.append(pClient);
    }


    void QtSubscribeClientImpl::subscribePriceDepthEvent(
            const char *symbols,
            const std::function<void(const PriceDepthEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::priceDepthChannel(symbols);
        m_url = m_marketUrl;

        auto pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedPriceDepthEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalPriceDepthEvent, this, callback);

        m_pClientList.append(pClient);
    }

    void QtSubscribeClientImpl::subscribe24HTradeStatisticsEvent(
            const char *symbols,
            const std::function<void(const TradeStatisticsEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::tradeStatisticsChannel(symbols);
        m_url = m_marketUrl;

        auto pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedTradeStatisticsEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalTradeStatisticsEvent, this, callback);

        m_pClientList.append(pClient);
    }

    void QtSubscribeClientImpl::subscribeOrderUpdateEvent(
            const char *symbols,
            const std::function<void(const OrderUpdateEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::orderUpdateChannel(symbols);
        QString authTopic = QtChannel::authChannel(m_host, m_accessKey, m_secretKey);
        m_url = m_tradeUrl;
        auto pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics,authTopic);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedOrderUpdateEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalOrderUpdateEvent, this, callback);

        m_pClientList.append(pClient);
    }

    void QtSubscribeClientImpl::subscribeAccountEvent(
            const BalanceMode &mode,
            const std::function<void(const AccountEvent &)> &callback,
            const std::function<void(HuobiApiException &)> &errorHandler) {
        QList<QString> topics = QtChannel::accountChannel(mode);
        QString authTopic = QtChannel::authChannel(m_host, m_accessKey, m_secretKey);
        m_url = m_tradeUrl;
        auto pClient = new QtWebSocketClient(QUrl(QString::fromStdString(m_url)), topics, authTopic);
        connect(pClient, &QtWebSocketClient::received, this, &QtSubscribeClientImpl::onReceivedAccountEvent);
        connect(this, &QtSubscribeClientImpl::callbackSignalAccountEvent, this, callback);

        m_pClientList.append(pClient);
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
        data.amount = QtDecimalTool::toDecimal(tick["amount"]);
        data.close = QtDecimalTool::toDecimal(tick["close"]);
        data.high = QtDecimalTool::toDecimal(tick["high"]);
        data.low = QtDecimalTool::toDecimal(tick["low"]);
        data.open = QtDecimalTool::toDecimal(tick["open"]);
        data.volume = QtDecimalTool::toDecimal(tick["vol"]);
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
                trade.amount = QtDecimalTool::toDecimal(item["amount"]);
                trade.price = QtDecimalTool::toDecimal(item["price"]);
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
        statistics.amount = QtDecimalTool::toDecimal(tick["amount"]);
        statistics.open = QtDecimalTool::toDecimal(tick["open"]);
        statistics.close = QtDecimalTool::toDecimal(tick["close"]);
        statistics.high = QtDecimalTool::toDecimal(tick["high"]);
        statistics.timestamp = ts;
        statistics.count = tick["count"].toULongLong();
        statistics.low = QtDecimalTool::toDecimal(tick["low"]);
        statistics.volume = QtDecimalTool::toDecimal(tick["vol"]);
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
        order.amount = QtDecimalTool::toDecimal(data["order-amount"]);
        order.price = QtDecimalTool::toDecimal(data["order-price"]);
        order.createdTimestamp = TimeService::convertCSTInMillisecondToUTC(data["created-at"].toULongLong());
        order.type = OrderType::lookup(data["order-type"].toString().toStdString());
        order.filledAmount = QtDecimalTool::toDecimal(data["filled-amount"]);
        order.filledCashAmount = QtDecimalTool::toDecimal(data["filled-cash-amount"]);
        order.filledFees = QtDecimalTool::toDecimal(data["filled-fees"]);
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
                change.balance = QtDecimalTool::toDecimal(item["balance"]);
                change.balanceType = BalanceType::lookup(item["type"].toString().toStdString());
                event.accountChangeList.push_back(change);
            }


        cout << "发送信号" << endl;
        emit callbackSignalAccountEvent(event);
    }
}






