#ifndef CMOUSEMONITORWIDGET_H
#define CMOUSEMONITORWIDGET_H

#include <Windows.h>
#include <QList>
#include <QPoint>

class XYMouseMonitor
{
    enum CLICKTYPE {DOWN, UP, MOVE, WHEEL};
public:
    explicit XYMouseMonitor(bool acceptManage = true);
    virtual ~XYMouseMonitor();
    static void startMouseMonitor();
    static void stopMouseMonitor();

protected:
    virtual void clicked(const QPoint &point) = 0;
    virtual void mouseEvent(WPARAM wParam,LPARAM lParam) {} // 子类需要的重载获取信息
    static QList<XYMouseMonitor *> mlistAllWidget;

private:
    static LRESULT CALLBACK LowLevelMouseProc(int nCode,WPARAM wParam,LPARAM lParam);
};

#endif // CMOUSEMONITORWIDGET_H
