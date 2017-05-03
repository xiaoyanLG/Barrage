#include "cbarragescreen.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QLibrary>
#include <Windows.h>

CBarrageScreen *CBarrageScreen::mopInstance = NULL;
CBarrageScreen::CBarrageScreen(QWidget *parent) : QWidget(parent)
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

CBarrageScreen::~CBarrageScreen()
{

}

CBarrageScreen *CBarrageScreen::getScreen()
{
    if (mopInstance == NULL)
    {
        mopInstance = new CBarrageScreen;
    }
    return mopInstance;
}

bool CBarrageScreen::forceTop()
{
    return mbForceTop;
}

void CBarrageScreen::addItem(CBarrageItem *item)
{
    int insert_pos = qMin(mlistBarrageItems.size(), miMaxBarrageNumber);
    mlistBarrageItems.insert(insert_pos, item);
    miRefreshTimer = startTimer(15);
    raise();
}

void CBarrageScreen::delItem(CBarrageItem *item)
{
    delete item;
    mlistBarrageItems.removeAll(item);
    if (mlistBarrageItems.size() == 0)
    {
        update();
        killTimer(miRefreshTimer);
    }
}

void CBarrageScreen::setMaxBarrageNumber(int max)
{
    miMaxBarrageNumber = max;
}

void CBarrageScreen::changeForceTop()
{
    mbForceTop = !mbForceTop;
}

void CBarrageScreen::setForceTop(bool top)
{
    mbForceTop = top;
}

void CBarrageScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter;
    painter.begin(this);

    for (int i = 0; i < mlistBarrageItems.size() && i < miMaxBarrageNumber; ++i)
    {
        CBarrageItem *item = mlistBarrageItems.at(i);
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
        Contents *contents = item->mopContents;
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
            case Contents::TEXT:
                painter.drawText(QRect(currentPos.x() + offset_X,
                                       currentPos.y() + offset_Y,
                                       limit_X < 0 ? 0:limit_X,
                                       limit_Y < 0 ? 0:limit_Y),
                                 contents->text);
                offset_X += contents->text.toLatin1().length()
                        * item->moTextFont.pointSize() + 2;
                current_Y = qMax(current_Y, item->moTextFont.pointSize() + 2.0);
                break;
            case Contents::LF:
                offset_Y += current_Y;
                offset_X = 0;
                break;
            case Contents::PIXMAP:
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

void CBarrageScreen::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == miRefreshTimer)
    {
        update();
    }
}

