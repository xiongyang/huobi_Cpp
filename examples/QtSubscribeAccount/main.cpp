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

    client->subscribeAccountEvent(BalanceMode::available, [](AccountEvent accountEvent) {
        cout << "---- Account Change: " << accountEvent.changeType.getValue() << " ----" << endl;
        for (AccountChange change : accountEvent.accountChangeList) {
            cout << "Account: " << change.accountType.getValue();
            cout << "Currency: " << change.currency << endl;
            cout << "Balance: " << change.balance << endl;
            cout << "Balance type: " << change.balanceType.getValue() << endl;
        }
    });
    return a.exec();
}
