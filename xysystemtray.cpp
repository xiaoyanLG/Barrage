#include "xysystemtray.h"
#include "xybarragescreen.h"
#include "xysignalbarragescreen.h"
#include "mainwindow.h"
#include "xymenu.h"
#include "xytooltips.h"
#include <QTime>
#include <QAction>
#include <QApplication>

void test()
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    XYBarrageScreen *screen = XYBarrageScreen::getScreen();
    for (int i = 0; i < 0; ++i)
    {
        XYBarrageItem *item = new XYBarrageItem(new XYContents(QStringLiteral("我就是来打酱油的~")), 8000, "green");
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

XYSystemTray::XYSystemTray(QWidget *parent)
    : QSystemTrayIcon(parent)
{
    InitTyay();
    test();
}

XYSystemTray::~XYSystemTray()
{

}
void XYSystemTray::InitTyay()
{
    connect(this,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(SystemTrayActivated(QSystemTrayIcon::ActivationReason)));

    this->setIcon(QIcon(":/send.ico"));
    this->setToolTip(QStringLiteral("给你惊喜！！！"));
}

void XYSystemTray::hideBarrageScreen()
{
    XYBarrageScreen::getScreen()->setHidden(!XYBarrageScreen::getScreen()->isHidden());
}

void XYSystemTray::closeAllAnimation()
{
    XYSignalBarrageScreen::deleteAllAnimations();
}

void XYSystemTray::showParent()
{
    QWidget *parent = (QWidget *)this->parent();
    if (parent)
    {
        parent->show();
        parent->activateWindow();
    }
}

void XYSystemTray::showContext()
{
    static XYMenu *myMenu = NULL;
    if (myMenu == NULL)
    {
        myMenu = new XYMenu;
        QAction *closeBarrageScreen = new QAction(QStringLiteral("关闭弹幕窗口"), myMenu);
        closeBarrageScreen->setCheckable(true);
        closeBarrageScreen->setChecked(XYBarrageScreen::getScreen()->isHidden());
        connect(closeBarrageScreen, SIGNAL(triggered()), this, SLOT(hideBarrageScreen()));

        QAction *top = new QAction(QStringLiteral("强制置顶"), myMenu);
        top->setCheckable(true);
        top->setChecked(XYBarrageScreen::getScreen()->forceTop());
        connect(top, SIGNAL(triggered()), XYBarrageScreen::getScreen(), SLOT(changeForceTop()));

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

void XYSystemTray::SystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
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
