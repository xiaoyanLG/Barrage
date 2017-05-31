#include "jsonparse.h"

JSONParse::JSONParse()
{

}

bool JSONParse::init(QString &json)
{

    QJsonParseError error;
    QJsonDocument jsonDoc =
            QJsonDocument::fromJson(json.toUtf8(),&error);
    if(error.error == QJsonParseError::NoError)
    {//将jsonDocument转换为数组
        this->jsonDocVar = jsonDoc.toVariant();
        return true;
    }
    return false;
}

QString JSONParse::getJsonValue(const QStringList &key)
{
    if(jsonDocVar.isNull())
        return " ";
    QVariantMap jsonArr =  jsonDocVar.toMap();
    return jsonValueFind(jsonArr,key);
}


QString JSONParse::jsonValueFind(QVariantMap jsonMap,const QStringList &key)
{

    int i = 0;
    for( i = 0;i < key.count()-1;i++)
    {
        int pos = -1;
        if((pos = jsonMap.keys().indexOf(key[i])) != -1)
        {
            jsonMap = jsonMap.values().at(pos).toMap();
        }
        else
        {
            return " ";
        }
    }
    return jsonMap[key[i]].toString();
}
