#include "xymenu.h"
#include "xymenustyle.h"
#include <QAction>
#include <QEventLoop>
#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <Windows.h>

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

    raise();
    if (!mlistWidgets.contains(this))
    {
        mlistWidgets.append(this);
    }
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

void XYMenu::raise()
{
    QWidget::raise();
    QWidget::setFocus();
    // 这里直接使用QT的setfocus函数无效，改用系统API（windows）
    SetFocus((HWND)this->winId());
}

void XYMenu::updateCheckedMenu()
{
    for (int i = 0; i < mlistWidgets.size(); ++i)
    {
        mlistWidgets.at(i)->update();
    }
}

int XYMenu::exec(XYMenu *parent)
{
    // 判断是否传入空指针
    if (parent == NULL)
    {
        return exec();
    }
    setupUI();
    QPoint pos = QCursor::pos();
    pos.setX(parent->pos().x() + parent->width() - 25);
    pos.setY(pos.y() - 15);
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
        pos.setY(pos.y() - height - 15);
    }
    move(pos);
    return mopEventLoop->exec();
}

bool XYMenu::close(bool closeParent)
{
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
        mopParentMenu->raise();
    }

    // 最后判断是否需要关闭父菜单
    if (mopParentMenu && closeParent)
    {
        return mopParentMenu->close(closeParent);
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
    if (menu != NULL)
    {
        menu->mopParentMenu = this;
        mlistWidgets += menu->mlistWidgets;
        mlistMenus.insert(actions().size(), menu);
    }
}

void XYMenu::focusOutEvent(QFocusEvent *event)
{
    QWidget *active = qApp->activeWindow();
    if (!mlistWidgets.contains(active)
            && !rect().contains(QCursor::pos() - pos()))
    {
        close();
    }
}

void XYMenu::leaveEvent(QEvent *event)
{
    bool b_close = true;
    if (mopParentMenu)
    {
        XYMenu *menu = mopParentMenu;
        do
        {
            if (menu->rect().contains(QCursor::pos() - menu->pos()))
            {
                b_close = false;
                break;
            }
            menu = menu->mopParentMenu;
        }while (menu);
    }

    auto it = mlistMenus.begin();
    while (it != mlistMenus.end())
    {
        XYMenu *menu = it.value();
        if (menu->rect().contains(QCursor::pos() - menu->pos()))
        {
            b_close = false;
            break;
        }
        it++;
    }

    if (b_close)
    {
        close(true);
    }
    event->accept();
}

void XYMenu::execMenu2(XYMenuStyle *style)
{
    if (style->mbIsMenu && style->mopMenu)
    {
        if (style->mopMenu->isHidden())
        {
            style->mopMenu->exec(this);
        }
        else
        {
            style->mopMenu->raise();
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

        if (!mlistWidgets.contains(style))
        {
            mlistWidgets.append(style);
        }
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

        if (!mlistWidgets.contains(style))
        {
            mlistWidgets.append(style);
        }

        it++;
        index++;
    }

    // 设置菜单大小，否则显示不出来
    for (int i = 0; i < mopMainLayout->count(); ++i)
    {
        mopMainLayout->itemAt(i)->widget()->setFixedSize(miActionMaxWidth + 80, 30);
    }
}

