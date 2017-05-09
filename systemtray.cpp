#include "systemtray.h"
#include "cbarragescreen.h"
#include <QTime>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include "csignalbarragescreen.h"
#include "mainwindow.h"
#include "xymenu.h"
#include "xytooltips.h"

void test()
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    CBarrageScreen *screen = CBarrageScreen::getScreen();
    for (int i = 0; i < 0; ++i)
    {
        CBarrageItem *item = new CBarrageItem(new Contents(QStringLiteral("我就是来打酱油的~")), 8000, "green");
        QColor c(qrand()%256,qrand()%256,qrand()%256);
        QFont font("幼圆");
        font.setPointSize(10 + qrand()%40);
        item->setTextFont(font);
        item->setTextColor(c);
        item->setShowTimes(qrand() % 10 * 1000 + 5000);
        item->setStartPos(QPoint(screen->width(), qMin(qrand()%54 * 15, screen->height() - 40)));
        item->setAnimation(QEasingCurve::Type(qrand() % 41));
        screen->addItem(item);
    }
    screen->show();
}

SystemTray::SystemTray(QWidget *parent)
    : QSystemTrayIcon(parent)
{
    InitTyay();
    test();
}

SystemTray::~SystemTray()
{

}
void SystemTray::InitTyay()
{
    connect(this,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(SystemTrayActivated(QSystemTrayIcon::ActivationReason)));

    this->setIcon(QIcon(":/send.ico"));
    this->setToolTip(QStringLiteral("给你惊喜！！！"));
}

void SystemTray::hideBarrageScreen()
{
    CBarrageScreen::getScreen()->setHidden(!CBarrageScreen::getScreen()->isHidden());
}

void SystemTray::closeAllAnimation()
{
    CSignalBarrageScreen::closeAllWidget();
}

void SystemTray::showParent()
{
    QWidget *parent = (QWidget *)this->parent();
    if (parent)
    {
        parent->show();
        parent->activateWindow();
    }
}

void SystemTray::showContext()
{
    static XYMenu *myMenu = NULL;
    if (myMenu == NULL)
    {
        myMenu = new XYMenu;
        QAction *closeBarrageScreen = new QAction(QStringLiteral("关闭弹幕窗口"), myMenu);
        closeBarrageScreen->setCheckable(true);
        closeBarrageScreen->setChecked(CBarrageScreen::getScreen()->isHidden());
        connect(closeBarrageScreen, SIGNAL(triggered()), this, SLOT(hideBarrageScreen()));

        QAction *top = new QAction(QStringLiteral("强制置顶"), myMenu);
        top->setCheckable(true);
        top->setChecked(CBarrageScreen::getScreen()->forceTop());
        connect(top, SIGNAL(triggered()), CBarrageScreen::getScreen(), SLOT(changeForceTop()));

        QAction *closeAllAnimations = new QAction(QStringLiteral("关闭所有动画"), myMenu);
        connect(closeAllAnimations, SIGNAL(triggered()), this, SLOT(closeAllAnimation()));

        QAction *quitAction = new QAction(QStringLiteral("不要我了。。。(退出)"), myMenu);

        connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

        myMenu->addAction(top);
        myMenu->addAction(closeBarrageScreen);
        myMenu->addAction(closeAllAnimations);
        myMenu->addAction(quitAction);
        myMenu->exec();
    }
    else if (myMenu->isHidden())
    {
        myMenu->exec();
    }
}

void SystemTray::SystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Context:
    {
        showContext();
        break;
    }
    case QSystemTrayIcon::DoubleClick:
    {
        showParent();
        break;
    }
    case QSystemTrayIcon::Trigger:
    {
        showParent();
        break;
    }
    default:
        break;
    }
}
