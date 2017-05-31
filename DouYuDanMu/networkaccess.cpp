#include "networkaccess.h"

NetworkAccess::NetworkAccess(QObject *parent):
    QObject(parent)
{
    codec = QTextCodec::codecForName("UTF-8");
    QObject::connect(&manager,SIGNAL(finished(QNetworkReply*)),
                     this,SLOT(replyFinished(QNetworkReply*)));
}

NetworkAccess::~NetworkAccess()
{

}

void NetworkAccess::loadingPage(QUrl &pageUrl,QTextCodec *codec)
{

    if(codec != NULL)
        this->codec = codec;
    request.setUrl(pageUrl);
    manager.get(request);
}


void NetworkAccess::replyFinished(QNetworkReply *reply)
{
    QByteArray replyByte = reply->readAll();
    QString html = this->codec->toUnicode(replyByte);
    emit pageLoadFinished(html);
}
