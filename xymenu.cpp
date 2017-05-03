#include "xymenu.h"
#include <QDebug>
#include <QEventLoop>
#include <QApplication>
#include <QDesktopWidget>

XYMenu::XYMenu(QWidget *parent)
    : XYBorderShadowWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::WindowType_Mask);
    mopEventLoop = new QEventLoop(this);
    setFocus();
}

XYMenu::~XYMenu()
{
    qDebug() << __FUNCTION__;
}

int XYMenu::exec()
{
    QPoint pos = QCursor::pos();
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
    show();
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

void XYMenu::focusOutEvent(QFocusEvent *event)
{
    close();
}

