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
    client->getLatestCandlestick("btcusdt", CandlestickInterval::min1, 150, 0, 0, [](vector<Candlestick> candelves) {
        cout << "---- 1 min candlestick for btcusdt ----" << endl;

        for (Candlestick candlestick : candelves) {
            cout << "Timestamp: " << candlestick.timestamp << endl;
            cout << "High: " << candlestick.high << endl;
            cout << "Low: " << candlestick.low << endl;
            cout << "Open: " << candlestick.open << endl;
            cout << "Close: " << candlestick.close << endl;
            cout << "Volume: " << candlestick.volume << endl;
        }
    });

    return a.exec();
}

