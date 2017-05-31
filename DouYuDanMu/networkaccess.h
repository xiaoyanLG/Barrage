#ifndef NETWORKACCESS_H
#define NETWORKACCESS_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QTextCodec>
#include <QUrl>
#include <QWidget>
#include <QObject>

/**
 * @brief The NetworkAccess class
 * 网络访问部分
 */
class NetworkAccess:public QObject
{
     Q_OBJECT
public:
    NetworkAccess(QObject *parent = 0);
    ~NetworkAccess();
signals:
    /**
     * @brief pageLoadFinished 页面加载完成
     * @param html
     */
    void pageLoadFinished(QString);
public slots:
    /**
     * @brief loadingPage 根据给出的网址加载数据
     * @param pageUrl
     * @param codec 网页编码格式 默认UTF-8,最终都会转为UTF-8
     */
    void loadingPage(QUrl &pageUrl,QTextCodec *codec = NULL);

private:
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QTextCodec *codec;


private slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // NETWORKACCESS_H
