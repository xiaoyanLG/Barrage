#include "xymousemonitor.h"
#include <Windows.h>

static HHOOK hhkMouse = NULL;			// 鼠标钩子句柄
QList<XYMouseMonitor *>  XYMouseMonitor::mlistAllWidget;

XYMouseMonitor::XYMouseMonitor(bool acceptManage)
{
    if (acceptManage)
    {
        mlistAllWidget.append(this);
    }
    startMouseMonitor();
}

XYMouseMonitor::~XYMouseMonitor()
{
    mlistAllWidget.removeAll(this);
}

void XYMouseMonitor::startMouseMonitor()
{
    // 注册鼠标监控函数
    if (hhkMouse == NULL)
    {
        hhkMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    }
}

void XYMouseMonitor::stopMouseMonitor()
{
    UnhookWindowsHookEx(hhkMouse);
    hhkMouse = NULL;
}

LRESULT XYMouseMonitor::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static PMSLLHOOKSTRUCT mouseHookStruct;
    // 鼠标位置
    mouseHookStruct = (PMSLLHOOKSTRUCT)lParam;
    // 所在窗口
    HWND hwnd = ::WindowFromPoint(mouseHookStruct->pt);
    TCHAR title[MAX_PATH];
    ::GetWindowText(hwnd,title,MAX_PATH);

    switch (wParam)
    {
    case WM_LBUTTONDOWN: // 鼠标左键按下
    case WM_RBUTTONDOWN: // 鼠标右键按下
    case WM_MBUTTONDOWN: // 鼠标中键按下
    {
        auto it = mlistAllWidget.begin();
        while (it != mlistAllWidget.end())
        {
            XYMouseMonitor *mon = (XYMouseMonitor *)*it;
            mon->clicked(QPoint(mouseHookStruct->pt.x, mouseHookStruct->pt.y));
            it++;
        }
        break;
    }
    case WM_MBUTTONUP: // 鼠标中键弹起
    case WM_LBUTTONUP: // 鼠标左键弹起
    case WM_RBUTTONUP: // 鼠标右键弹起
        break;
    case WM_MOUSEMOVE: // 鼠标移动
    case WM_MOUSEWHEEL: // 鼠标滚动
        break;
    default:
        break;
    }

    auto it = mlistAllWidget.begin();
    while (it != mlistAllWidget.end())
    {
        XYMouseMonitor *mon = (XYMouseMonitor *)*it;
        mon->mouseEvent(wParam, lParam);
        it++;
    }
    return CallNextHookEx(hhkMouse,nCode,wParam,lParam);
}
