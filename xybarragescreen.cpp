#include "xybarragescreen.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QMessageBox>
#include <Windows.h>
#include <QDebug>
XYBarrageScreen *XYBarrageScreen::mopInstance = NULL;
XYBarrageScreen::XYBarrageScreen(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::WindowType_Mask);
    this->setAttribute(Qt::WA_TranslucentBackground);

    mbForceTop = false;
    miMaxBarrageNumber = 30;
    miRefreshTimer = startTimer(15);
    resize(QApplication::desktop()->width(), QApplication::desktop()->height());
    SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
                   WS_EX_TRANSPARENT | WS_EX_LAYERED);
}

XYBarrageScreen::~XYBarrageScreen()
{

}

XYBarrageScreen *XYBarrageScreen::getScreen()
{
    if (mopInstance == NULL)
    {
        mopInstance = new XYBarrageScreen;
    }
    return mopInstance;
}

bool XYBarrageScreen::forceTop()
{
    return mbForceTop;
}

void XYBarrageScreen::insertItem(XYBarrageItem *item, int pos)
{
    if (mlistBarrageItems.size() > 1000) // 如果缓冲区存在太多的话就不要再添加，防止内存占用太多
    {
        delete item;
        return;
    }
    int insert_pos = miMaxBarrageNumber + pos;
    if (mlistBarrageItems.size() > insert_pos)
    {
        mlistBarrageItems.insert(insert_pos, item);
    }
    else
    {
        mlistBarrageItems.append(item);
    }
    if (miRefreshTimer == 0)
    {
        miRefreshTimer = startTimer(15);
    }
}

void XYBarrageScreen::addItem(XYBarrageItem *item)
{
    insertItem(item, mlistBarrageItems.size());
}

void XYBarrageScreen::delItem(XYBarrageItem *item)
{
    delete item;
    mlistBarrageItems.removeAll(item);
    if (mlistBarrageItems.size() == 0)
    {
        update();
        killTimer(miRefreshTimer);
        miRefreshTimer = 0;
    }
}

void XYBarrageScreen::setMaxBarrageNumber(int max)
{
    miMaxBarrageNumber = max;
}

void XYBarrageScreen::changeForceTop()
{
    mbForceTop = !mbForceTop;
}

void XYBarrageScreen::setForceTop(bool top)
{
    mbForceTop = top;
}

void XYBarrageScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter;
    painter.begin(this);

    QList<QRect> rects;
    for (int i = 0; i < mlistBarrageItems.size() && i < miMaxBarrageNumber; ++i)
    {
        XYBarrageItem *item = mlistBarrageItems.at(i);
        QFontMetrics metrics(item->moTextFont);
        if (!rects.isEmpty() && !item->isAdjust()) // 防止弹幕覆盖处理
        {
            QPoint pos = item->getCurrentPos(false);
            int limit = this->height();
            while (rectCovered(QRect(pos.x(), pos.y(), 1, item->getRealContentsHeight()), rects) && limit)
            {
                if (pos.y() + 50 > this->height())
                {
                    item->offsetY(50 - this->height());
                }
                else
                {
                    item->offsetY(1);
                }
                pos = item->getCurrentPos(false);
                item->setAdjust(true);
                limit--;
            }
        }

        QPoint currentPos = item->getCurrentPos();
        painter.setPen(item->moTextColor);
        painter.setFont(item->moTextFont);
        painter.setOpacity(item->getCurrentOpacity());
        rects.append(QRect(currentPos.x(), currentPos.y(),
                           item->getRealContentsWidth(), item->getRealContentsHeight()));
        if (item->miBarrageWidth == -1)
        {
            item->miBarrageWidth = QApplication::desktop()->width();
        }
        if (item->miBarrageHeight == -1)
        {
            item->miBarrageHeight = QApplication::desktop()->height();
        }

        QPixmap pixmap = item->moBackImage.currentPixmap();
        if (!pixmap.isNull())
        {
            painter.drawPixmap(QRect(currentPos.x(),
                                     currentPos.y(),
                                     qMin(item->miBarrageWidth, pixmap.width()),
                                     qMin(item->miBarrageHeight, pixmap.height())),
                               pixmap,
                               pixmap.rect());
        }
        XYContents *contents = item->mopContents;
        qreal offset_X = 0;
        qreal offset_Y = 0;
        qreal lineHeight = 0;
        int limit_X = 0;
        int limit_Y = 0;
        XYContents *lineContents = contents;
        while (contents)
        {
            limit_X = item->miBarrageWidth - offset_X;
            limit_Y = item->miBarrageHeight - offset_Y;
            switch (contents->type)
            {
            case XYContents::TEXT:
            {
                offset_X += metrics.width(contents->text);
                lineHeight = qMax(lineHeight, metrics.height() * 1.0);
                break;
            }
            case XYContents::LF:
            {
                offset_X = 0;
                paintLineContents(painter, item, lineContents,
                                  currentPos, lineHeight, offset_X, offset_Y);
                offset_Y += lineHeight;
                lineContents = contents->next;
                lineHeight = 0;
                break;
            }
            case XYContents::PIXMAP:
            {
                QPixmap pixmap;
                if (!contents->pixmap.isValid())
                {
                    QString filename = contents->pixmap.fileName();
                    pixmap.load(filename);
                }
                else if (contents->pixmap.state() != QMovie::Running)
                {
                    contents->pixmap.start();
                    pixmap = contents->pixmap.currentPixmap();
                }
                else
                {
                    pixmap = contents->pixmap.currentPixmap();
                }
                offset_X += pixmap.width();
                lineHeight = qMax(lineHeight, pixmap.height() * 1.0);
                break;
            }
            default:
                break;
            }

            contents = contents->next;
        }

        if (item->isFinished())
        {
            delItem(item);
        }
    }
    if (mbForceTop)
    {
        raise();
    }
    painter.end();
}

void XYBarrageScreen::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == miRefreshTimer)
    {
        update();
    }
}

void XYBarrageScreen::paintLineContents(QPainter &painter,
                                        XYBarrageItem *item,
                                        XYContents *lineContents,
                                        QPoint &currentPos,
                                        qreal lineHeight,
                                        qreal offsetX,
                                        qreal offsetY)
{
    qreal offset_X = offsetX;
    qreal offset_Y = offsetY;
    int limit_X = 0;
    int limit_Y = 0;
    XYContents *contents = lineContents;
    while (contents)
    {
        limit_X = item->miBarrageWidth - offset_X;
        limit_Y = item->miBarrageHeight - offset_Y;
        switch (contents->type)
        {
        case XYContents::TEXT:
        {
            QFontMetrics metrics(item->moTextFont);
            painter.drawText(QRect(currentPos.x() + offset_X,
                                   currentPos.y() + offset_Y + lineHeight - metrics.height(),
                                   limit_X < 0 ? 0:limit_X,
                                   limit_Y < 0 ? 0:limit_Y),
                             contents->text);
            offset_X += metrics.width(contents->text);
            break;
        }
        case XYContents::LF:
            return;
        case XYContents::PIXMAP:
        {
            QPixmap pixmap;
            if (!contents->pixmap.isValid())
            {
                QString filename = contents->pixmap.fileName();
                pixmap.load(filename);
            }
            else if (contents->pixmap.state() != QMovie::Running)
            {
                contents->pixmap.start();
                pixmap = contents->pixmap.currentPixmap();
            }
            else
            {
                pixmap = contents->pixmap.currentPixmap();
            }

            painter.drawPixmap(QRect(currentPos.x() + offset_X,
                                     currentPos.y() + offset_Y + lineHeight - pixmap.height(),
                                     qMin(limit_X < 0 ? 0:limit_X, pixmap.width()),
                                     qMin(limit_Y < 0 ? 0:limit_Y, pixmap.height())),
                               pixmap,
                               QRect(0,
                                     0,
                                     qMin(limit_X < 0 ? 0:limit_X, pixmap.width()),
                                     qMin(limit_Y < 0 ? 0:limit_Y, pixmap.height())));
            offset_X += pixmap.width();
            break;
        }
        default:
            break;
        }

        contents = contents->next;
    }
}

bool XYBarrageScreen::rectCovered(const QRect &rect, const QList<QRect> &rects)
{
    for (int i = 0; i < rects.size(); ++i)
    {
        if (!rects.at(i).intersected(rect).isEmpty())
        {
            return true;
        }
    }
    return false;
}
