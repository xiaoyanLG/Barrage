#ifndef CMOUSEMONITORWIDGET_H
#define CMOUSEMONITORWIDGET_H

#include <QWidget>
#include <Windows.h>

class CMouseMonitorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CMouseMonitorWidget(QWidget *parent = 0);
    ~CMouseMonitorWidget();
    static void startMouseMonitor();
    static void stopMouseMonitor();
    static void setCurrentInstance(CMouseMonitorWidget *widget);
    static void closeAllWidget();

public slots:
    virtual void clicked(QPoint point) = 0;
    static void changeRandomCall();

protected:
    static QList<CMouseMonitorWidget *> mlistAllWidget;
    static bool   mbRandomCall;

private:
    static LRESULT CALLBACK LowLevelMouseProc(int nCode,WPARAM wParam,LPARAM lParam);
};

#endif // CMOUSEMONITORWIDGET_H
