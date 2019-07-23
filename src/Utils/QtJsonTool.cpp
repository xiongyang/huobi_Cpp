#include "QtJsonTool.h"
#include <QtCore/QDebug>
#include <QByteArray>

QT_USE_NAMESPACE

namespace Huobi {
    QtJsonTool::QtJsonTool(QObject *parent) :
            QObject(parent) {
    }

    QtJsonTool::~QtJsonTool() {

    }

    QJsonObject QtJsonTool::getJsonObjectFromByteArray(const QByteArray buffer) {
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(buffer, &error);
        QJsonObject jsonObject;
        if (error.error == QJsonParseError::NoError) {
            if (jsonDocument.isObject()) {
                jsonObject = jsonDocument.object();
            }
        } else {
            qDebug() << "无法将字符串转为json" << endl;
        }

        return jsonObject;
    }

     QString QtJsonTool::getStringFromJsonObject(const QJsonObject &jsonObject) {
        return QString(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
    }
}