#include "cmousemonitorwidget.h"
#include <Windows.h>
#include <QTime>
#include <QDebug>

static HHOOK hhkMouse = NULL;			// 鼠标钩子句柄
static CMouseMonitorWidget *instance = NULL;
QList<CMouseMonitorWidget *> CMouseMonitorWidget::mlistAllWidget;
bool CMouseMonitorWidget::mbRandomCall = true;

CMouseMonitorWidget::CMouseMonitorWidget(QWidget *parent)
    : QWidget(parent)
{
    instance = this;
    mlistAllWidget.append(this);
    startMouseMonitor();
}

CMouseMonitorWidget::~CMouseMonitorWidget()
{
    mlistAllWidget.removeAll(this);
    if (!mlistAllWidget.isEmpty())
    {
        instance = mlistAllWidget.last();
    }
}

void CMouseMonitorWidget::startMouseMonitor()
{
    // 注册鼠标监控函数
    if (hhkMouse == NULL)
    {
        hhkMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    }
}

void CMouseMonitorWidget::stopMouseMonitor()
{
    UnhookWindowsHookEx(hhkMouse);
    hhkMouse = NULL;
}

void CMouseMonitorWidget::setCurrentInstance(CMouseMonitorWidget *widget)
{
    instance = widget;
}

void CMouseMonitorWidget::closeAllWidget()
{
    instance = NULL;
    while (!mlistAllWidget.isEmpty())
    {
        delete mlistAllWidget.at(0);
    }
    stopMouseMonitor();
}

void CMouseMonitorWidget::changeRandomCall()
{
    mbRandomCall = !mbRandomCall;
}

LRESULT CMouseMonitorWidget::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static PMSLLHOOKSTRUCT mouseHookStruct;
    // 鼠标位置
    mouseHookStruct = (PMSLLHOOKSTRUCT)lParam;
    // 所在窗口
    HWND hwnd = ::WindowFromPoint(mouseHookStruct->pt);
    TCHAR title[MAX_PATH];
    ::GetWindowText(hwnd,title,MAX_PATH);

    if (wParam == WM_LBUTTONDBLCLK)	// 鼠标左键按下
    {
//        qDebug() << QStringLiteral("鼠标左键按下");
    }
    else if (wParam == WM_LBUTTONUP) // 鼠标左键弹起
    {
//        qDebug() << QStringLiteral("鼠标左键弹起");
//        qDebug() << QString::fromStdWString(title) << QPoint(mouseHookStruct->pt.x, mouseHookStruct->pt.y);
        qsrand(QTime::currentTime().msecsTo(QTime(0, 0)));
        if (mbRandomCall)
        {
            for (int i = 0; i < mlistAllWidget.size(); ++i)
            {
                if (qrand() % 2)
                {
                    mlistAllWidget.at(i)->clicked(QPoint(mouseHookStruct->pt.x, mouseHookStruct->pt.y));
                }
            }
        }
        else
        {
            instance->clicked(QPoint(mouseHookStruct->pt.x, mouseHookStruct->pt.y));
        }
    }
    else if ( wParam == WM_RBUTTONDBLCLK) // 鼠标右键按下
    {
//        qDebug() << QStringLiteral("鼠标右键按下");
    }
    else if ( wParam == WM_RBUTTONUP) // 鼠标右键弹起
    {
//        qDebug() << QStringLiteral("鼠标右键弹起");
    }
    else if ( wParam == WM_MOUSEMOVE) // 鼠标移动
    {
//        qDebug() << QStringLiteral("鼠标移动");
    }
    else if ( wParam == WM_MOUSEWHEEL) // 鼠标滚动
    {
//        qDebug() << QStringLiteral("鼠标滚动");
    }

    return CallNextHookEx(hhkMouse,nCode,wParam,lParam);
}
