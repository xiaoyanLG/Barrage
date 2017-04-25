#include "cbarrageitem.h"
#include "cbarragescreen.h"
#include <QPropertyAnimation>
#include <QtMath>
#include <QDebug>
CBarrageItem::CBarrageItem(Contents *contents,
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
    mfOpactiy  = 0;
    miLoopCounts = 1;
    miBarrageWidth  = -1;
    miBarrageHeight = -1;
    mfCurrentX = moStartPos.x();
    mfCurrentY = moStartPos.y();
    setShowTimes(showTimes, miLoopCounts);
}

CBarrageItem::~CBarrageItem()
{
    qDebug() << QStringLiteral("析构弹幕内容");
    Contents *contents = mopContents;
    while (contents)
    {
        Contents *next = contents->next;
        delete contents;
        contents = next;
    }
}

QPoint CBarrageItem::getCurrentPos()
{
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

qreal CBarrageItem::getCurrentOpacity()
{
    return getOpactiy();
}

void CBarrageItem::setAnimation(const QEasingCurve &type)
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
        mopOpactiyAnimation->setKeyValueAt(500.0/miShowTimes, 1);
    }
    if (miShowTimes <= 1000)
    {
        mopOpactiyAnimation->setKeyValueAt(0.5, 1);
    }
    if (miShowTimes > 1000)
    {
        mopOpactiyAnimation->setKeyValueAt(1.0- 500.0/miShowTimes, 1);
    }
    mopOpactiyAnimation->setEndValue(0);
}

QPoint CBarrageItem::getPos() const
{
//    qDebug() << moCurrentPos;
    return moCurrentPos;
}

void CBarrageItem::setPos(const QPoint &pos)
{
    if (moCurrentPos != pos)
    {
        moCurrentPos = pos;
    }
}

qreal CBarrageItem::getOpactiy() const
{
//    qDebug() << mfOpactiy;
    return mfOpactiy;
}

void CBarrageItem::setOpactiy(qreal opactiy)
{
    if (mfOpactiy != opactiy)
    {
        mfOpactiy = opactiy;
    }
}

bool CBarrageItem::isFinished()
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

qreal CBarrageItem::lengthToPoint(const QPoint &p1, const QPoint &p2)
{
    qreal x = qFabs(p1.x() - p2.x());
    qreal y = qFabs(p1.y() - p2.y());
    return qSqrt((x*x) + (y*y));
}

void CBarrageItem::setShowTimes(int showTimes, int counts)
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

void CBarrageItem::setContents(Contents *contents)
{
    mopContents = contents;
}

void CBarrageItem::setTextColor(const QColor &textColor)
{
    moTextColor = textColor;
}

void CBarrageItem::setTextFont(const QFont &textFont)
{
    moTextFont = textFont;
}

void CBarrageItem::setStartPos(const QPoint &startPos)
{
    moStartPos = startPos;
    if (moStartPos.isNull())
    {
        CBarrageScreen *screen = CBarrageScreen::getScreen();
        if (screen)
        {
            moStartPos = QPoint(screen->width(), 50);
        }
    }
    mfCurrentX = moStartPos.x();
    mfCurrentY = moStartPos.y();
    if (moEndPos.isNull())
    {
        moEndPos = QPoint(-msContents.toLocal8Bit().length() * moTextFont.pointSize()
                          , moStartPos.y());
    }
    setShowTimes(miShowTimes, miLoopCounts);
}

void CBarrageItem::setEndPos(const QPoint &endPos)
{
    moEndPos = endPos;
    if (moEndPos.isNull())
    {
        moEndPos = QPoint(- msContents.toLocal8Bit().length() * moTextFont.pointSize()
                          , moStartPos.y());
    }
    setShowTimes(miShowTimes, miLoopCounts);
}

void CBarrageItem::setBarrageSize(int barrageWidth, int barrageHeight)
{
    miBarrageWidth  = barrageWidth;
    miBarrageHeight = barrageHeight;
}

void CBarrageItem::setDrawPoints(const QList<QPoint> &points)
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

void CBarrageItem::setBackImage(const QMovie &image)
{
    if (image.isValid())
    {
        moBackImage.setFileName(image.fileName());
        moBackImage.start();
    }
}
