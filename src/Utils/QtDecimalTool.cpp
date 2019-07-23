#include "QtDecimalTool.h"
#include <iostream>
#include <QtCore/QDebug>
#include <QtCore/QString>


using namespace std;

QT_USE_NAMESPACE

namespace Huobi {


    QtDecimalTool::QtDecimalTool(QObject *parent) :
            QObject(parent) {
    }

    QtDecimalTool::~QtDecimalTool() {

    }

    Decimal QtDecimalTool::toDecimal(QVariant num) {
        return Decimal(num.toString().toStdString().c_str());
    }
}






