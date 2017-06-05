#include "xybarrageitem.h"
#include "xybarragescreen.h"
#include <QPropertyAnimation>
#include <QtMath>
#include <QDebug>

XYBarrageItem::XYBarrageItem(XYContents *contents,
                           int showTimes,
                           const QColor &textColor,
                           const QFont &textFont,
                           int barrageWidth)
     :mopContents(contents),
      miShowTimes(showTimes),
      moTextColor(textColor),
      moTextFont(textFont),
      miBarrageWidth(barrageWidth)
{
    mopMoveAnimation    = NULL;
    mopOpactiyAnimation = NULL;
    mbUseUserDrawPoints = false;
    mbStarted  = false;
    mbAdjust   = false;
    mfOpactiy  = 1;
    miLoopCounts = 1;
    miBarrageWidth  = -1;
    miBarrageHeight = -1;
    mfCurrentX = moStartPos.x();
    mfCurrentY = moStartPos.y();
    setShowTimes(showTimes, miLoopCounts);
}

XYBarrageItem::~XYBarrageItem()
{
    deleteContents();
}

QPoint XYBarrageItem::getCurrentPos(bool start)
{
    if (!start)
    {
        return moStartPos;
    }
    if (mopMoveAnimation == NULL)
    {
        if (!mbEqualX)
        {
            mfCurrentX += mfSetpX;
        }
        else
        {
            mfCurrentX = moStartPos.x();
        }
        if (!mbEqualY)
        {
            mfCurrentY += mfSetpY;
        }
        else
        {
            mfCurrentY = moStartPos.y();
        }
        mbStarted = true;
        return QPoint(mfCurrentX, mfCurrentY);
    }
    else
    {
        if (!mbStarted && mopOpactiyAnimation && mopMoveAnimation)
        {
            mbStarted = true;
            if (mbUseUserDrawPoints)
            {
                mopMoveAnimation->setStartValue(moStartPos);
                qreal elapsed = 0;
                for (int i = 1; i < mlistDrawPoints.size() - 1 && i < mlistPointSpaces.size(); ++i)
                {
                    elapsed += mlistPointSpaces.at(i) / mfTotalLength;
                    mopMoveAnimation->setKeyValueAt(elapsed
                                                    , mlistDrawPoints.at(i));
                }
                mopMoveAnimation->setEndValue(moEndPos);
            }
            else
            {
                mopMoveAnimation->setStartValue(moStartPos);
                mopMoveAnimation->setEndValue(moEndPos);
            }
            mopOpactiyAnimation->start();
            mopMoveAnimation->start();
        }

        return getPos();
    }
}

qreal XYBarrageItem::getCurrentOpacity()
{
    return getOpactiy();
}

int XYBarrageItem::getContentsWidth()
{
    int width = this->miBarrageWidth;
    if (width == -1)
    {
        width = getRealContentsWidth();
    }

    return width;
}

int XYBarrageItem::getContentsHeight()
{
    int height = this->miBarrageHeight;
    if (height == -1)
    {
        height = getRealContentsHeight();
    }

    return height;
}

int XYBarrageItem::getRealContentsWidth()
{
    int width = 0;
    XYContents *contents = this->mopContents;
    int w = 0;
    while (contents)
    {
        switch (contents->type)
        {
        case XYContents::TEXT:
        {
            QFontMetrics metrics(this->moTextFont);
            w += metrics.width(contents->text) + 2;
            break;
        }
        case XYContents::LF:
            width = qMax(w, width);
            w = 0;
            break;
        case XYContents::PIXMAP:
            if (!contents->pixmap.isValid())
            {
                QString filename = contents->pixmap.fileName();
                w += QPixmap(filename).width() + 1;
            }
            else if (contents->pixmap.state() != QMovie::Running)
            {
                contents->pixmap.start();
                w += contents->pixmap.currentPixmap().width() + 1;
            }
            else
            {
                w += contents->pixmap.currentPixmap().width() + 1;
            }
            break;
        default:
            break;
        }
        contents = contents->next;
    }
    return width;
}

int XYBarrageItem::getRealContentsHeight()
{
    int height = 0;
    XYContents *contents = this->mopContents;
    int h = 0;
    while (contents)
    {
        switch (contents->type)
        {
        case XYContents::TEXT:
        {
            QFontMetrics metrics(this->moTextFont);
            h = qMax(h, metrics.height() + 2);
            break;
        }
        case XYContents::LF:
            height += h;
            h = 0;
            break;
        case XYContents::PIXMAP:
            if (!contents->pixmap.isValid())
            {
                QString filename = contents->pixmap.fileName();
                h = qMax(h, QPixmap(filename).height() + 1);
            }
            else if (contents->pixmap.state() != QMovie::Running)
            {
                contents->pixmap.start();
                h = qMax(h, contents->pixmap.currentPixmap().height() + 1);
            }
            else
            {
                h = qMax(h, contents->pixmap.currentPixmap().height() + 1);
            }
            break;
        default:
            break;
        }
        contents = contents->next;
    }

    return height;
}

bool XYBarrageItem::isAdjust()
{
    if (!mbAdjust && mbStarted && lengthToPoint(moCurrentPos, moStartPos) > 5)
    {
        mbAdjust = true;
    }
    return mbAdjust;
}

void XYBarrageItem::setAdjust(bool adjust)
{
    mbAdjust = adjust;
}

void XYBarrageItem::offsetY(int offset)
{
    if (moStartPos.y() == moEndPos.y())
    {
        moStartPos.setY(moStartPos.y() + offset);
        moEndPos.setY(moEndPos.y() + offset);
    }
}

void XYBarrageItem::setAnimation(const QEasingCurve &type)
{
    mopMoveAnimation = new QPropertyAnimation(this, "pos");
    mopMoveAnimation->setLoopCount(miLoopCounts);
    mopMoveAnimation->setDuration(miShowTimes);
    mopMoveAnimation->setEasingCurve(type);

    mopOpactiyAnimation = new QPropertyAnimation(this, "opactiy");
    mopOpactiyAnimation->setLoopCount(miLoopCounts);
    mopOpactiyAnimation->setDuration(miShowTimes);
    mopOpactiyAnimation->setStartValue(0);
    if (miShowTimes > 1000)
    {
        mopOpactiyAnimation->setKeyValueAt(300.0/miShowTimes, 1);
    }
    if (miShowTimes <= 1000)
    {
        mopOpactiyAnimation->setKeyValueAt(0.5, 1);
    }
    if (miShowTimes > 1000)
    {
        mopOpactiyAnimation->setKeyValueAt(1.0- 300.0/miShowTimes, 1);
    }
    mopOpactiyAnimation->setEndValue(0);
}

QPoint XYBarrageItem::getPos() const
{
    return moCurrentPos;
}

void XYBarrageItem::setPos(const QPoint &pos)
{
    if (moCurrentPos != pos)
    {
        moCurrentPos = pos;
    }
}

qreal XYBarrageItem::getOpactiy() const
{
    return mfOpactiy;
}

void XYBarrageItem::setOpactiy(qreal opactiy)
{
    if (mfOpactiy != opactiy)
    {
        mfOpactiy = opactiy;
    }
}

bool XYBarrageItem::isFinished()
{
    if (mopMoveAnimation != NULL)
    {
        return mopMoveAnimation->state() != QAbstractAnimation::Running;
    }
    if (moEndPos.x() > moStartPos.x())
    {
        return mfCurrentX >= moEndPos.x();
    }
    else if (moEndPos.x() < moStartPos.x())
    {
        return mfCurrentX <= moEndPos.x();
    }
    else
    {
        if (moEndPos.y() > moStartPos.y())
        {
            return mfCurrentY >= moEndPos.y();
        }
        else if (moEndPos.y() < moStartPos.y())
        {
            return mfCurrentY <= moEndPos.y();
        }
        else
        {
            return true;
        }
    }
}

qreal XYBarrageItem::lengthToPoint(const QPoint &p1, const QPoint &p2)
{
    qreal x = qFabs(p1.x() - p2.x());
    qreal y = qFabs(p1.y() - p2.y());
    return qSqrt((x*x) + (y*y));
}

void XYBarrageItem::deleteContents()
{
    XYContents *contents = mopContents;
    while (contents)
    {
        XYContents *next = contents->next;
        delete contents;
        contents = next;
    }
    mopContents = NULL;
}

void XYBarrageItem::setShowTimes(int showTimes, int counts)
{
    miShowTimes = showTimes;
    miLoopCounts = counts;
    if (moEndPos.x() != moStartPos.x())
    {
        mfSetpX = moEndPos.x() - moStartPos.x();
        mfSetpX = mfSetpX / miShowTimes * 10.0;
        mbEqualX = false;
    }
    else
    {
        mfSetpX = 0;
        mbEqualX = true;
    }

    if (moEndPos.y() != moStartPos.y())
    {
        mfSetpY = moEndPos.y() - moStartPos.y();
        mfSetpY = mfSetpY / miShowTimes * 10.0;
        mbEqualY = false;
    }
    else
    {
        mfSetpY = 0;
        mbEqualY = true;
    }
}

void XYBarrageItem::setContents(XYContents *contents)
{
    deleteContents();
    mopContents = contents;
    setEndPos(QPoint());
}

void XYBarrageItem::setTextColor(const QColor &textColor)
{
    moTextColor = textColor;
}

void XYBarrageItem::setTextFont(const QFont &textFont)
{
    moTextFont = textFont;
}

void XYBarrageItem::setStartPos(const QPoint &startPos)
{
    moStartPos = startPos;
    if (moStartPos.isNull())
    {
        XYBarrageScreen *screen = XYBarrageScreen::getScreen();
        if (screen)
        {
            moStartPos = QPoint(screen->width(), 50);
        }
    }
    mfCurrentX = moStartPos.x();
    mfCurrentY = moStartPos.y();
    if (moEndPos.isNull())
    {
        moEndPos = QPoint(- getRealContentsWidth()
                          , moStartPos.y());
    }
    setShowTimes(miShowTimes, miLoopCounts);
}

void XYBarrageItem::setEndPos(const QPoint &endPos)
{
    moEndPos = endPos;
    if (moEndPos.isNull())
    {
        moEndPos = QPoint(- getRealContentsWidth()
                          , moStartPos.y());
    }
    setShowTimes(miShowTimes, miLoopCounts);
}

void XYBarrageItem::setBarrageSize(int barrageWidth, int barrageHeight)
{
    miBarrageWidth  = barrageWidth;
    miBarrageHeight = barrageHeight;
}

void XYBarrageItem::setDrawPoints(const QList<QPoint> &points)
{
    mlistDrawPoints = points;
    if (!mlistDrawPoints.isEmpty())
    {
        mbUseUserDrawPoints = true;
        mfTotalLength = 0;
        moStartPos = mlistDrawPoints.first();
        moEndPos   = mlistDrawPoints.last();

        QPoint prevPoint = mlistDrawPoints.first();
        for (int i = 0; i < mlistDrawPoints.size(); ++i)
        {
            qreal length = lengthToPoint(prevPoint, mlistDrawPoints.at(i));
            mlistPointSpaces.append(length);
            mfTotalLength += length;
        }
    }
}

void XYBarrageItem::setBackImage(const QMovie &image)
{
    if (image.isValid())
    {
        moBackImage.setFileName(image.fileName());
        moBackImage.start();
    }
}
