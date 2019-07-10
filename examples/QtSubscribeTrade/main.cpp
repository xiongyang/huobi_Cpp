/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>
#include "Huobi/HuobiClient.h"
#include <QtCore/QCoreApplication>


using namespace Huobi;
using namespace std;

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    SubscribeClient *client = createSubscribeClient();

    client->subscribeTradeEvent("btcusdt,ethusdt,htusdt,htbtc", [](TradeEvent tradeEvent) {

        cout << "Timestamp: " << tradeEvent.timestamp << endl;
        cout << "Trade: " << endl;
        for (Trade trade : tradeEvent.tradeList) {
            cout << "TradeId: " << trade.tradeId << endl;
            cout << "Timestamp: " << trade.timestamp << endl;
            cout << "Price: " << trade.price << endl;
            cout << "Amount: " << trade.amount << endl;
            cout << "Direction: " << trade.direction.getValue() << endl;
        }
    });
    return a.exec();
}
