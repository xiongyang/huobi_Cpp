/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>
#include "Huobi/HuobiClient.h"

using namespace Huobi;
using namespace std;

int main(int argc, char **argv) {
    SubscriptionClient *client = createSubscriptionClient();
    client->subscribePriceDepthEvent("btcusdt,ethusdt,htusdt,htbtc", [](PriceDepthEvent pdEvent) {
        cout << "Timestamp: " << pdEvent.timestamp << endl;
        cout << "Data: " << endl;
        cout << "Timestamp: " << pdEvent.data.timestamp << endl;
        cout << "bids: " << endl;
        for (DepthEntry depth : pdEvent.data.bids) {
            cout << "Price: " << depth.price;
            cout << "Amount: " << depth.amount << endl;
        }
        cout << "asks: " << endl;
        for (DepthEntry depth : pdEvent.data.asks) {
            cout << "Price: " << depth.price;
            cout << "Amount: " << depth.amount << endl;
        }
    });

    client->startService();
}

