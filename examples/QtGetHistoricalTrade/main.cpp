/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Huobi/HuobiClient.h"
#include<iostream>
#include<vector>
#include <QtCore/QCoreApplication>
#include <QDebug>

using namespace Huobi;
using namespace std;

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    RestClient *client = createRestClient();

    client->getHistoricalTrade("btcusdt", 5, [](vector<Trade> tradeVes) {
        cout << "---- Historical trade for btcusdt ----" << endl;
        for (Trade trade : tradeVes) {
            cout << "Trade at: " << trade.timestamp << endl;
            cout << "Id: " << trade.tradeId << endl;
            cout << "Price: " << trade.price << endl;
            cout << "Amount: " << trade.amount << endl;
            cout << "Direction: " << trade.direction.getValue() << endl;
        }
    });

    return a.exec();
}

