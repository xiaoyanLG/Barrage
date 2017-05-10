#include "xybarragescreen.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QMessageBox>
#include <Windows.h>

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

void XYBarrageScreen::addItem(XYBarrageItem *item)
{
    int insert_pos = qMin(mlistBarrageItems.size(), miMaxBarrageNumber);
    mlistBarrageItems.insert(insert_pos, item);
    miRefreshTimer = startTimer(15);
    raise();
}

void XYBarrageScreen::delItem(XYBarrageItem *item)
{
    delete item;
    mlistBarrageItems.removeAll(item);
    if (mlistBarrageItems.size() == 0)
    {
        update();
        killTimer(miRefreshTimer);
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

    for (int i = 0; i < mlistBarrageItems.size() && i < miMaxBarrageNumber; ++i)
    {
        XYBarrageItem *item = mlistBarrageItems.at(i);
        QPoint currentPos = item->getCurrentPos();
        painter.setPen(item->moTextColor);
        painter.setFont(item->moTextFont);
        painter.setOpacity(item->getCurrentOpacity());
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
        qreal current_Y = 0;
        int limit_X = 0;
        int limit_Y = 0;
        while (contents)
        {
            limit_X = item->miBarrageWidth - offset_X;
            limit_Y = item->miBarrageHeight - offset_Y;
            switch (contents->type)
            {
            case XYContents::TEXT:
            {
                painter.drawText(QRect(currentPos.x() + offset_X,
                                       currentPos.y() + offset_Y,
                                       limit_X < 0 ? 0:limit_X,
                                       limit_Y < 0 ? 0:limit_Y),
                                 contents->text);
                QFontMetrics metrics(item->moTextFont);
                offset_X += metrics.width(contents->text) + 2;
                current_Y = qMax(current_Y, metrics.height() + 2.0);
                break;
            }
            case XYContents::LF:
                offset_Y += current_Y;
                offset_X = 0;
                break;
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
                                         currentPos.y() + offset_Y,
                                         qMin(limit_X < 0 ? 0:limit_X, pixmap.width()),
                                         qMin(limit_Y < 0 ? 0:limit_Y, pixmap.height())),
                                   pixmap,
                                   QRect(0,
                                         0,
                                         qMin(limit_X < 0 ? 0:limit_X, pixmap.width()),
                                         qMin(limit_Y < 0 ? 0:limit_Y, pixmap.height())));
                offset_X += pixmap.width() + 2;
                current_Y = qMax(current_Y, pixmap.height() + 2.0);
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

