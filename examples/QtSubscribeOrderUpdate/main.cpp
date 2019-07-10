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
    SubscribeClient *client = createSubscribeClient("xxxx", "xxxx");

    client->subscribeOrderUpdateEvent("btcusdt,ethusdt,htusdt,htbtc", [](OrderUpdateEvent ouEvent) {

        cout << "Timestamp: " << ouEvent.timestamp << endl;
        cout << "data: " << endl;
        cout << "AccountType: " << ouEvent.data.accountType.getValue() << endl;
        cout << "Amount: " << ouEvent.data.amount << endl;
        cout << "Price: " << ouEvent.data.price << endl;
        cout << "CreatedTimestamp: " << ouEvent.data.createdTimestamp << endl;
        cout << "CanceledTimestamp: " << ouEvent.data.canceledTimestamp << endl;
        cout << "FinishedTimestamp: " << ouEvent.data.finishedTimestamp << endl;
        cout << "OrderId: " << ouEvent.data.orderId << endl;
        cout << "Type: " << ouEvent.data.type.getValue() << endl;
        cout << "FilledAmount: " << ouEvent.data.filledAmount << endl;
        cout << "FilledCashAmount: " << ouEvent.data.filledCashAmount << endl;
        cout << "FilledFees: " << ouEvent.data.filledFees << endl;
        cout << "Source: " << ouEvent.data.source.getValue() << endl;
        cout << "State: " << ouEvent.data.state.getValue() << endl;
    });
    return a.exec();
}

