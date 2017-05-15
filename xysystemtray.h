#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>

class XYSystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit XYSystemTray(QWidget *parent = 0);
    ~XYSystemTray();
    void InitTyay();

private slots:
    void hideBarrageScreen();
    void closeAllAnimation();
    void showParent();
    void showContext();
    void SystemTrayActivated(QSystemTrayIcon::ActivationReason reason);//处理点击托盘操作

private:
    //截图全局热键
    int      miShortcutKey;
};

#endif // SYSTEMTRAY_H
