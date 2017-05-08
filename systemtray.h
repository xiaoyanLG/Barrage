#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = 0);
    ~SystemTray();
    void InitTyay();

private slots:
    void hideBarrageScreen();
    void closeAllAnimation();
    void showParent();
    void showContext();
    void SystemTrayActivated(QSystemTrayIcon::ActivationReason reason);//处理点击托盘操作

signals:
    void showWidget();
};

#endif // SYSTEMTRAY_H
