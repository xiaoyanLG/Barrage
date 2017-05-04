#include "xymenustyle.h"
#include <QPainter>

int XYMenuStyle::mimaxWidth = 0;
XYMenuStyle::XYMenuStyle(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(100, 30);
    mbContainsMouse = false;
}

XYMenuStyle::XYMenuStyle(XYAction *action, QWidget *parent)
    :QWidget(parent)
{
    mopAction = action;
    mbContainsMouse = false;
    if (mopAction == NULL)
    {
        mopAction = new XYAction;
    }

    QFontMetrics metrics(mopAction->font());
    int width = metrics.width(mopAction->text());
    mimaxWidth = mimaxWidth > width ? mimaxWidth : width;
    setFixedSize(30 + 25 + mimaxWidth, 30);
}

XYMenuStyle::~XYMenuStyle()
{

}

void XYMenuStyle::paintEvent(QPaintEvent *event)
{
    setFixedSize(30 + 25 + mimaxWidth, 30);
    int width = this->width();
    int height = this->height();
    QPainter painter(this);
    QPen pen = painter.pen();
    painter.setPen(QPen(QColor(0, 0, 0, 0)));
    QFont font = painter.font();

    // 画底色
    if (mbContainsMouse)
    {
        painter.setBrush(Qt::blue);
    }
    else
    {
        painter.setBrush(QColor("#F0F0F0"));
    }
    painter.drawRect(this->rect());

    // 画图标
    QPixmap pixmap(":/Gif2/0");
    pixmap.scaled(QSize(25, 25),
                  Qt::KeepAspectRatio,
                  Qt::SmoothTransformation);
    painter.drawPixmap(QRect(10,
                             (height - pixmap.height()) / 2,
                             pixmap.width(),
                             pixmap.height()),
                       pixmap,
                       pixmap.rect());

    // 画字体
    painter.setFont(mopAction->font());
    painter.setPen(pen);
    painter.drawText(QRect(pixmap.width() + 20,
                           (height - font.pointSize()) / 2,
                           width,
                           height),
                     mopAction->text());
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

