#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QCoreApplication>
#include <QJsonObject>
#include <QDebug>

#include "util.h"

JsonReader::JsonReader()
{
    QString currPath = QCoreApplication::applicationDirPath();
    QString filePath = currPath + "/" + "configForCpp.json";
    QFile loadFile(filePath);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open json file!";
    }
    QByteArray allData = loadFile.readAll();
    loadFile.close();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));
    if (jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error: " << jsonError.errorString();
    }

    QJsonObject rootObj = jsonDoc.object();
    QStringList keys = rootObj.keys();
    for (int i=0; i < keys.size(); i++)
    {
        m_configMap.insert(keys.at(i), rootObj.value(keys.at(i)).toString());
    }
}

QString JsonReader::getValue(const QString &key)
{
    if (m_configMap.contains(key))
        return m_configMap.value(key);

    return "";
}
