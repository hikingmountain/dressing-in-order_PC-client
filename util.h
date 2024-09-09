#ifndef UTIL_H
#define UTIL_H

#include <QMap>

class JsonReader
{
public:
    JsonReader();
    QString getValue(const QString& key);

private:
    QMap<QString, QString> m_configMap;

};

#endif // UTIL_H
