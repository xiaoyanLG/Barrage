#include "xymenustyle.h"
#include <QPainter>
#include <QDebug>

int XYMenuStyle::mimaxWidth = 0;
XYMenuStyle::XYMenuStyle(XYAction *action, QWidget *parent)
    :QWidget(parent)
{
    mopAction = action;
    mopMenu = NULL;
    mbContainsMouse = false;
    mbIsMenu = false;

    setMaxWidth();
}

XYMenuStyle::XYMenuStyle(XYMenu *menu, QWidget *parent)
    :QWidget(parent)
{
    mopAction = NULL;
    mopMenu = menu;
    mbContainsMouse = false;
    mbIsMenu = true;

    setMaxWidth();
}

XYMenuStyle::~XYMenuStyle()
{

}

void XYMenuStyle::paintEvent(QPaintEvent *event)
{
    setFixedSize(30 + 25 + mimaxWidth + 25, 30);
    int width = this->width();
    int height = this->height();
    QPainter painter(this);
    QPen pen = painter.pen();
    painter.setPen(QPen(QColor(0, 0, 0, 0)));
    QFont font = painter.font();

    // 画底色
    if (mbContainsMouse)
    {
        painter.setBrush(QColor("#2871d5"));
    }
    else
    {
        painter.setBrush(QColor("#F0F0F0"));
    }
    painter.drawRect(this->rect());

    if (!mbIsMenu)
    {
        // 画图标
        QPixmap pixmap(mopAction->icon().pixmap(25, 25));
        painter.drawPixmap(QRect(10,
                                 (height - pixmap.height()) / 2,
                                 pixmap.width(),
                                 pixmap.height()),
                           pixmap,
                           pixmap.rect());

        // 画字体
        painter.setFont(mopAction->font());
        painter.setPen(pen);
        painter.drawText(QRect(25 + 20,
                               (height - font.pointSize()) / 2,
                               width,
                               height),
                         mopAction->text());

        // 画是否勾选
        if (mopAction->isCheckable())
        {
            QString s_ico;
            if (mopAction->isChecked())
            {
                s_ico = ":/sourceImage/checked";
            }
            else
            {
                s_ico = ":/sourceImage/unchecked";
            }
            QPixmap checkico(s_ico);
            checkico = checkico.scaled(QSize(15, 15),
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);
            painter.drawPixmap(QRect(25 + 20 + mimaxWidth + 10,
                                     (height - checkico.height()) / 2,
                                     checkico.width(),
                                     checkico.height()),
                               checkico,
                               checkico.rect());
        }
    }
    else
    {
        // 画图标
        QPixmap pixmap(mopMenu->icon().pixmap(25, 25));
        painter.drawPixmap(QRect(10,
                                 (height - pixmap.height()) / 2,
                                 pixmap.width(),
                                 pixmap.height()),
                           pixmap,
                           pixmap.rect());

        // 画字体
        painter.setFont(mopMenu->font());
        painter.setPen(pen);
        painter.drawText(QRect(25 + 20,
                               (height - font.pointSize()) / 2,
                               width,
                               height),
                         mopMenu->title());

        // 画下级菜单图标
        QString s_ico = ":/sourceImage/menu2";
        QPixmap menuico(s_ico);
        menuico = menuico.scaled(QSize(15, 15),
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);
        painter.drawPixmap(QRect(25 + 20 + mimaxWidth + 15,
                                 (height - menuico.height()) / 2,
                                 menuico.width(),
                                 menuico.height()),
                           menuico,
                           menuico.rect());
    }
}

void XYMenuStyle::enterEvent(QEvent *event)
{
    mbContainsMouse = true;
    update();
}

void XYMenuStyle::leaveEvent(QEvent *event)
{
    mbContainsMouse = false;
    update();
}

void XYMenuStyle::mouseReleaseEvent(QMouseEvent *event)
{
    if (!mbIsMenu)
    {
        if (!mopAction->isCheckable())
        {
            ((QWidget *)parent())->close();
        }
        else
        {
            mopAction->setChecked(!mopAction->isChecked());
            update();
        }
        emit mopAction->triggered(mopAction->isChecked());
    }
}

void XYMenuStyle::setMaxWidth()
{
    if (mbIsMenu)
    {
        QFontMetrics metrics(mopMenu->font());
        int width = metrics.width(mopMenu->title());
        mimaxWidth = mimaxWidth > width ? mimaxWidth : width;
        setFixedSize(30 + 25 + mimaxWidth + 25, 30);
    }
    else
    {
        QFontMetrics metrics(mopAction->font());
        int width = metrics.width(mopAction->text());
        mimaxWidth = mimaxWidth > width ? mimaxWidth : width;
        setFixedSize(30 + 25 + mimaxWidth + 25, 30);
    }
}

