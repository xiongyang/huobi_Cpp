#ifndef QTJSONTOOL_H
#define QTJSONTOOL_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <iostream>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QByteArray>


using namespace std;

namespace Huobi {
    class QtJsonTool : public QObject {
    Q_OBJECT
    public:
        explicit QtJsonTool(QObject *parent = nullptr);
        ~QtJsonTool();

    public:
        static QJsonObject getJsonObjectFromByteArray(const QByteArray buffer);

        static QString getStringFromJsonObject(const QJsonObject &jsonObject);
    };
}
#endif // QTJSONTOOL_H

