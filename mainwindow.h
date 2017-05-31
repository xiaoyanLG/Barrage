#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMap>
#include <QMovie>

// 斗鱼弹幕
#include "douyutcpsocket.h"
#include "networkaccess.h"

namespace Ui {
class MainWindow;
}

class XYBarrageItem;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow *instance();
    explicit MainWindow(QWidget *parent = 0);
    QFont getFont();

    ~MainWindow();
    XYBarrageItem *getItem(int time = 0);

public slots:
    void addItem();
    void addItem(int time);
    void getPoints();
    void setBackImage();
    void insertImage(const QString &filename);
    void insertImage();
    void timerShot(int number);
    void addAnimation();

    // 斗鱼弹幕获取接口
public slots:
    void htmlContent(const QString html);
    void showChatMessage(QMap<QString, QString>);
    void start();
    void stop();

protected:
    void timerEvent(QTimerEvent *event);

private:
    static  MainWindow  *mopInstance;
    Ui::MainWindow      *ui;
    int                  miTimer;
    int                  miLoopShot;

    QMovie               moBackImage;
    qreal                mfTotalLength;
    QList<QPoint>        mlistDrawPoints;
    QList<qreal>         mlistPointSpaces;

    // 斗鱼弹幕
    NetworkAccess *network_access;
    DouyuTcpSocket *tcpSocket;
};

#endif // MAINWINDOW_H
