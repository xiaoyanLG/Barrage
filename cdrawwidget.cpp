#include "cdrawwidget.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtMath>
#include <QDebug>

DrawWidget::DrawWidget(QWidget *parent)
    : QDialog(parent)
{
    mbPressed = false;
    mbClear   = false;
    mfTotalLength = 0;
    QPushButton *box = new QPushButton("Clear");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(box, 0, Qt::AlignRight);
    layout->addStretch();
    setLayout(layout);
    connect(box, SIGNAL(clicked()), this, SLOT(clearPoints()));
}

DrawWidget::~DrawWidget()
{

}

QList<QPoint> &DrawWidget::getDrawPoints()
{
    return mlistPointList;
}

QList<qreal> &DrawWidget::getDrawPointSpaces()
{
    return mlistPointSpaceList;
}

qreal DrawWidget::getTotalLength()
{
    return mfTotalLength;
}

qreal DrawWidget::lengthToPoint(const QPoint &p1, const QPoint &p2)
{
    qreal x = qFabs(p1.x() - p2.x());
    qreal y = qFabs(p1.y() - p2.y());
    return qSqrt((x*x) + (y*y));
}

void DrawWidget::clearPoints()
{
    mlistPointList.clear();
    mlistPointSpaceList.clear();
    moLastPixMap = QPixmap();
    mbClear = true;
    mbPressed = false;
    update();
}

void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (event->buttons() & Qt::LeftButton)
    {
        mbPressed = true;
        moCurrentPoint = event->pos();
        moLastPixMap = grab();
        update();
        moLastPoint = moCurrentPoint;
        qreal length = 0;
        if (!mlistPointList.isEmpty())
        {
            length = lengthToPoint(mlistPointList.last(), moCurrentPoint);
        }
        mfTotalLength += length;
        mlistPointList.append(moCurrentPoint);
        mlistPointSpaceList.append(length);
    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (event->buttons() & Qt::LeftButton)
    {
        mbPressed = false;
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (mbPressed)
    {
        moCurrentPoint = event->pos();
        moLastPixMap = grab();
        update();
        moLastPoint = moCurrentPoint;
        qreal length = 0;
        if (!mlistPointList.isEmpty())
        {
            length = lengthToPoint(mlistPointList.last(), moCurrentPoint);
        }
        mfTotalLength += length;
        mlistPointList.append(moCurrentPoint);
        mlistPointSpaceList.append(length);
    }
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.drawPixmap(0, 0, moLastPixMap);
    painter.save();
    painter.setFont(QFont("黑体", 30));
    painter.drawText(width() / 2 - 200, 50, QStringLiteral("按Esc键退出并保存轨迹！"));
    painter.restore();
    if (!mlistPointList.isEmpty())
    {
        painter.drawLine(moLastPoint, moCurrentPoint);
    }
    mbClear = false;
}

