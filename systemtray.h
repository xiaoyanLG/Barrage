#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QAction>
#include <QApplication>
#include <QMenu>

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = 0);
    ~SystemTray();
    void createAction();

private:
    QMenu *myMenu;//托盘菜单
    QAction *quitAction;//退出

private slots:
    void ShowParent();
    void SystemTrayActivated(QSystemTrayIcon::ActivationReason reason);//处理点击托盘操作

signals:
    void showWidget();
};

#endif // SYSTEMTRAY_H
