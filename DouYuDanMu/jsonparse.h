#ifndef JSONPARSE_H
#define JSONPARSE_H
#include <QtCore>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>

/**
 * @brief The JSONParse class
 * JSON文件分析类
 */
class JSONParse
{
public:
    JSONParse();
    /**
     * @brief init 初始化json语句
     * @param json
     */
    bool init(QString &json);

    /**
     * @brief getJsonValue 通过键值获取JsonValue
     * @param key
     * @return
     */
    QString getJsonValue(const QStringList &key);

private:
    QVariant jsonDocVar;

    /**
     * @brief jsonValueFind 查找json值
     * @param key
     * @return
     */
    QString jsonValueFind(QVariantMap jsonMap,const QStringList &key);

};

#endif // JSONPARSE_H
