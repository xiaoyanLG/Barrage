#include "barrage.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>
Barrage::Barrage(const QString &contents,
                 int showTimes,
                 const QColor &textColor,
                 const QFont &textFont,
                 const QPoint &startPos,
                 int barrageWidth)
    : QWidget(NULL),
      msContents(contents),
      miShowTimes(showTimes),
      moTextColor(textColor),
      moTextFont(textFont),
      moStartPos(startPos),
      miBarrageWidth(barrageWidth)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::WindowType_Mask);
    this->setAttribute(Qt::WA_TranslucentBackground);

    mopMoveAnimation = new QPropertyAnimation(this, "pos");

    connect(mopMoveAnimation, SIGNAL(finished()), this, SLOT(close()));
    connect(mopMoveAnimation, SIGNAL(finished()), this, SLOT(deleteLater()));

    resize(msContents.length() * moTextFont.pointSize(), 100);
}

Barrage::~Barrage()
{

}

void Barrage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter tempPainter;
    tempPainter.begin(this);
    QPen pen = tempPainter.pen();
    pen.setBrush(moTextColor);
    tempPainter.setPen(pen);
    tempPainter.setFont(moTextFont);
    if (miBarrageWidth == -1)
    {
        miBarrageWidth = msContents.length() * moTextFont.pointSize();
    }
    tempPainter.drawText(QRectF(0, 0, miBarrageWidth, 100), msContents);
    tempPainter.end();
}

void Barrage::setShowTimes(int showTimes)
{
    miShowTimes = showTimes;
}

void Barrage::setTextColor(const QColor &textColor)
{
    moTextColor = textColor;
}

void Barrage::setTextFont(const QFont &textFont)
{
    moTextFont = textFont;
}

void Barrage::setStartPos(const QPoint &startPos)
{
    moStartPos = startPos;
}

void Barrage::setBarrageWidth(int barrageWidth)
{
    miBarrageWidth = barrageWidth;
}

void Barrage::show()
{
    mopMoveAnimation->setDuration(miShowTimes);
    mopMoveAnimation->setStartValue(moStartPos);
    mopMoveAnimation->setEndValue(QPoint(-msContents.length() * moTextFont.pointSize()
                                         , moStartPos.y()));
    mopMoveAnimation->start();
    QWidget::show();
}

