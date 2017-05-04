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
    setFocus();
}

XYMenu::XYMenu(const QString &title, QWidget *parent)
    :XYMenu(parent)
{
    msTitle = title;
}

XYMenu::XYMenu(const QIcon &icon, const QString &title, QWidget *parent)
    :XYMenu(parent)
{
    msTitle = title;
    moIcon  = icon;
}

QString XYMenu::title()
{
    return msTitle;
}

QIcon XYMenu::icon()
{
    return moIcon;
}

QFont XYMenu::font()
{
    return moFont;
}

XYMenu::~XYMenu()
{
    qDebug() << __FUNCTION__;
}

int XYMenu::exec()
{
    setupUI();
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

void XYMenu::setTitle(const QString &title)
{
    msTitle = title;
}

void XYMenu::setIcon(const QIcon &icon)
{
    moIcon = icon;
}

void XYMenu::setFont(const QFont &font)
{
    moFont = font;
}

void XYMenu::addMenu(XYMenu *menu)
{
    mlistMenus.insert(actions().size(), menu);
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
    if (event->type() == QFocusEvent::FocusOut)
    {
        WId child = (WId)GetFocus();
        QWidget *cwd = find(child);
        if (!children().contains(cwd))
        {
            close();
        }
    }
}

void XYMenu::setupUI()
{
    auto actions = this->actions();
    for (int i = 0; i < actions.size(); ++i)
    {
        XYMenuStyle *style = new XYMenuStyle((XYAction *)actions.at(i));
        mopMainLayout->addWidget(style);
    }
    auto it = mlistMenus.begin();
    int index = 0;
    while (it != mlistMenus.end())
    {
        XYMenuStyle *style = new XYMenuStyle((XYMenu *)it.value());
        mopMainLayout->insertWidget(index + it.key(), style);
        it++;
        index++;
    }
}

