#include "xymenu.h"
#include "xymenustyle.h"
#include "xyaction.h"
#include <QDebug>
#include <QEventLoop>
#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets>

XYMenu::XYMenu(QWidget *parent)
    : XYBorderShadowWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::WindowType_Mask);
    mopEventLoop = new QEventLoop(this);
    mopMainLayout = new QVBoxLayout(this);
    mopMainLayout->setSpacing(0);

    XYMenuStyle *btn = new XYMenuStyle(new XYAction("55555"));
    XYMenuStyle *btn1 = new XYMenuStyle(new XYAction("6666666"));
    mopMainLayout->addWidget(btn);
    mopMainLayout->addWidget(btn1);

    setFocus();
}

XYMenu::~XYMenu()
{
    qDebug() << __FUNCTION__;
}

int XYMenu::exec()
{
    QPoint pos = QCursor::pos();
    show();
    // show 出来以后才能获取正确的窗口大小
    int width = this->width();
    int height = this->height();
    QDesktopWidget *top = QApplication::desktop();
    if (top->width() < width + pos.x())
    {
        pos.setX(top->width() - width - 5);
    }
    if (top->height() < height + pos.y())
    {
        pos.setY(top->height() - height - 5);
    }
    move(pos);
    return mopEventLoop->exec();
}

bool XYMenu::close()
{
    if (mopEventLoop != NULL)
    {
        mopEventLoop->exit();
    }
    return QWidget::close();
}

void XYMenu::childEvent(QChildEvent *event)
{
//    qDebug() << __FUNCTION__;
    if (event->added())
    {
        QObject *child = event->child();
        if (child->isWidgetType())
        {
            child->installEventFilter(this);
        }
    }
}

bool XYMenu::eventFilter(QObject *watched, QEvent *event)
{
//    qDebug() << __FUNCTION__;
    if (event->type() == QEvent::FocusOut)
    {
        focusOutEvent((QFocusEvent *)event);
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void XYMenu::focusOutEvent(QFocusEvent *event)
{
//    qDebug() << __FUNCTION__;
    QWidget *child = childAt(QCursor::pos() - pos());
    if (child == NULL)
    {
        close();
    }
}

