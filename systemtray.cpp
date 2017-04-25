#include "systemtray.h"
#include "cbarragescreen.h"
#include <QTime>
#include "csignalbarragescreen.h"
#include "mainwindow.h"

void test()
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    CBarrageScreen *screen = CBarrageScreen::getScreen();
    for (int i = 0; i < 10; ++i)
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
    createAction();
    test();
}

SystemTray::~SystemTray()
{

}
void SystemTray::createAction()
{
    quitAction = new QAction(QStringLiteral("不要我了。。。"),this);
    myMenu = new QMenu();

    this->setIcon(QIcon(":/send.ico"));
    this->setToolTip(QStringLiteral("给你惊喜！！！"));

    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(this,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(SystemTrayActivated(QSystemTrayIcon::ActivationReason)));

    myMenu->addAction(quitAction);
    this->setContextMenu(myMenu);
}

void SystemTray::ShowParent()
{
    QWidget *parent = (QWidget *)this->parent();
    if (parent)
    {
        parent->show();
        parent->activateWindow();
    }
}

void SystemTray::SystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    {
        ShowParent();
        break;
    }
    case QSystemTrayIcon::DoubleClick:
    {
        ShowParent();
        break;
    }
    default:
        break;
    }
}
