#include "xymenu.h"
#include "xymenustyle.h"
#include "xyaction.h"
#include <QDebug>
#include <QEventLoop>
#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets>

static int iglobal = 0;
XYMenu::XYMenu(QWidget *parent)
    : XYBorderShadowWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::WindowType_Mask);
    mopEventLoop = new QEventLoop(this);
    mopMainLayout = new QVBoxLayout(this);
    miActionMaxWidth = 0;
    mopParentMenu = NULL;
    mopMainLayout->setSpacing(0);
//    setFocus();
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
//    qDebug() << __FUNCTION__ << iglobal++;
}

int XYMenu::exec()
{
//    qDebug() << __FUNCTION__;
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

int XYMenu::exec(XYMenu *parent)
{
//    qDebug() << __FUNCTION__ << this<< iglobal++;
    // 判断是否传入空指针
    if (parent == NULL)
    {
        return exec();
    }
    setupUI();
    QPoint pos = QCursor::pos();
    pos.setX(parent->pos().x() + parent->width() - 25);
    pos.setY(pos.y() - 10);
    show();
    // show 出来以后才能获取正确的窗口大小
    int width = this->width();
    int height = this->height();
    QDesktopWidget *top = QApplication::desktop();
    if (top->width() < width + pos.x())
    {
        pos.setX(parent->pos().x() - width + 25);
    }
    if (top->height() < height + pos.y())
    {
        pos.setY(pos.y() - height + 30);
    }
    move(pos);
    return mopEventLoop->exec();
}

bool XYMenu::close(bool closeParent)
{
//    qDebug() << __FUNCTION__ << closeParent << this << iglobal++;
    if (mopEventLoop != NULL)
    {
        mopEventLoop->exit();
    }
    // 先关闭所有子菜单
    auto it = mlistMenus.begin();
    while (it != mlistMenus.end())
    {
        XYMenu *menu = it.value();
        menu->close();
        it++;
    }

    // 再关闭当前菜单
    bool ret = QWidget::close();

    // 先把焦点给父菜单
    if (mopParentMenu)
    {
        qDebug() << "++++++++++++++" << iglobal++ << mopParentMenu;
        mopParentMenu->raise();
        mopParentMenu->setFocus();
    }

    // 最后判断是否需要关闭父菜单
    if (mopParentMenu && closeParent)
    {
        mopParentMenu->close(closeParent);
    }
    else
    {
        return ret;
    }
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
    menu->mopParentMenu = this;
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
//    qDebug() << __FUNCTION__ << (void *)GetFocus();
    if (event->type() == QEvent::FocusOut)
    {
        focusOutEvent((QFocusEvent *)event);
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void XYMenu::focusOutEvent(QFocusEvent *event)
{
//    qDebug() << __FUNCTION__ << this << iglobal++;
    if (event->type() == QFocusEvent::FocusOut)
    {

        WId child = (WId)GetFocus();
        QWidget *cwd = find(child);
        if (mlistMenus.key((XYMenu *)cwd) == NULL)
        {
            if (mopParentMenu)
            {
                XYMenu *menu = mopParentMenu;
                bool b_close = true;
                do
                {
                    if (menu->rect().contains(QCursor::pos() - menu->pos()))
                    {
                        b_close = false;
                        break;
                    }
                    menu = menu->mopParentMenu;
                }while (menu);
                if (b_close)
                {
                    close(true);
                }
            }
            else
            {
                close();
            }
        }
    }
}

void XYMenu::execMenu2(XYMenuStyle *style)
{
//    qDebug() << __FUNCTION__ << this << iglobal++;
    if (style->mbIsMenu && style->mopMenu)
    {
        if (style->mopMenu->isHidden())
        {
            style->mopMenu->exec(this);
        }
        else
        {
            style->mopMenu->raise();
            style->mopMenu->setFocus();
        }
    }
    else
    {
        auto it = mlistMenus.begin();
        while (it != mlistMenus.end())
        {
            XYMenu *menu = it.value();
            if (!menu->isHidden())
            {
                menu->close();
            }
            it++;
        }
        raise();
    }
}

void XYMenu::setupUI()
{
    // 如果layout里面有控件，说明已经设置过内容，直接退出
    if (mopMainLayout->count() != 0)
    {
        return;
    }
    auto actions = this->actions();
    for (int i = 0; i < actions.size(); ++i)
    {
        QAction *action = actions.at(i);
        XYMenuStyle *style = new XYMenuStyle(action);
        QFontMetrics metrics(action->font());
        miActionMaxWidth = qMax(miActionMaxWidth, metrics.width(action->text()));
        mopMainLayout->addWidget(style);
    }
    auto it = mlistMenus.begin();
    int index = 0;
    while (it != mlistMenus.end())
    {
        XYMenu *menu = it.value();
        XYMenuStyle *style = new XYMenuStyle(menu);
        QFontMetrics metrics(menu->font());
        miActionMaxWidth = qMax(miActionMaxWidth, metrics.width(menu->title()));
        mopMainLayout->insertWidget(index + it.key(), style);
        it++;
        index++;
    }

    // 设置菜单大小，否则显示不出来
    for (int i = 0; i < mopMainLayout->count(); ++i)
    {
        mopMainLayout->itemAt(i)->widget()->setFixedSize(miActionMaxWidth + 80, 30);
    }
}

