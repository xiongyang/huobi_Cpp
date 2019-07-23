#ifndef QTDECIMALTOOL_H
#define QTDECIMALTOOL_H

#include <QtCore/QObject>
#include <iostream>
#include <string>
#include <Huobi/Decimal.h>


using namespace std;


namespace Huobi {
    class QtDecimalTool : public QObject {

    Q_OBJECT
    public:

        explicit QtDecimalTool(QObject *parent = nullptr);

        ~QtDecimalTool();

    public:
        static Decimal toDecimal(QVariant num);
    };
}


#endif /* QTDECIMALTOOL_H */

