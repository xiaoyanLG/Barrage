#include "csignalbarragescreen.h"
#include "xymenu.h"
#include "xytooltips.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDebug>
CSignalBarrageScreen::CSignalBarrageScreen(CBarrageItem *item, QWidget *parent)
    : CMouseMonitorWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::WindowType_Mask);
    this->setAttribute(Qt::WA_TranslucentBackground);

    mbLeftMousePressed = false;
    mbFixed = false;
    mbAutoMove = false;
    mbForceTop = false;
    mbMouseThrough = false;
    mopMoveAnimation = NULL;
    mopStepAnimation = NULL;
    mopContentsItem = NULL;
    miMoveAutoTimer = 0;
    setItem(item);
    miWindowLong = GetWindowLong((HWND)winId(), GWL_EXSTYLE);

    for (int i = 0; i < 5; ++i)
    {
        mmapPath.insert(i, true);
    }

    miRefreshTimer = startTimer(30);
}

CSignalBarrageScreen::~CSignalBarrageScreen()
{
    delete mopContentsItem;
}

qreal CSignalBarrageScreen::step() const
{
    return mfStep;
}

void CSignalBarrageScreen::setStep(qreal step)
{
    if (mfStep != step)
    {
        mfStep = step;
        QPoint point = moMovePath.getPoint(step);
        if (point.x() > QApplication::desktop()->width() - 50)
        {
            point.setX(QApplication::desktop()->width() - 50);
        }
        else if (point.x() < 0)
        {
            point.setX(0);
        }

        if (point.y() > QApplication::desktop()->height() - 100)
        {
            point.setY(QApplication::desktop()->height() - 100);
        }
        else if (point.y() < 0)
        {
            point.setY(0);
        }
        move(point);
    }
}

void CSignalBarrageScreen::setItem(CBarrageItem *item)
{
    if (mopContentsItem != NULL)
    {
        delete mopContentsItem;
    }
    mopContentsItem = item;
    if (mopContentsItem)
    {
        int width = mopContentsItem->miBarrageWidth;
        if (width == -1)
        {
            width = 0;
            Contents *contents = mopContentsItem->mopContents;
            int w = 0;
            while (contents)
            {
                switch (contents->type)
                {
                case Contents::TEXT:
                {
                    QFontMetrics metrics(mopContentsItem->moTextFont);
                    w += metrics.width(contents->text) + 2;
                    break;
                }
                case Contents::LF:
                    width = qMax(w, width);
                    w = 0;
                    break;
                case Contents::PIXMAP:
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
        }
        int height = mopContentsItem->miBarrageHeight;
        if (height == -1)
        {
            height = 0;
            Contents *contents = mopContentsItem->mopContents;
            int h = 0;
            while (contents)
            {
                switch (contents->type)
                {
                case Contents::TEXT:
                {
                    QFontMetrics metrics(mopContentsItem->moTextFont);
                    h = qMax(h, metrics.height() + 2);
                    break;
                }
                case Contents::LF:
                    height += h;
                    h = 0;
                    break;
                case Contents::PIXMAP:
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
        }
        resize(width, height);
        move(QApplication::desktop()->width() - width - 50
             , QApplication::desktop()->height() - height - 50);
    }
}

void CSignalBarrageScreen::changeFixed()
{
    mbFixed = !mbFixed;
    if (mbFixed)
    {
        if (mopMoveAnimation && mopMoveAnimation->state() == QPropertyAnimation::Running)
        {
            mopMoveAnimation->stop();
        }
    }
}

void CSignalBarrageScreen::changeForceTop()
{
    mbForceTop = !mbForceTop;
}

void CSignalBarrageScreen::changeAutoMove()
{
    mbAutoMove = !mbAutoMove;
    if (mbAutoMove)
    {
        if (miMoveAutoTimer == 0)
        {
            miMoveAutoTimer = startTimer(7000);
        }
    }
    else
    {
        killTimer(miMoveAutoTimer);
        miMoveAutoTimer = 0;
    }
}

void CSignalBarrageScreen::moveNextPoint()
{
    qsrand(QTime::currentTime().msecsTo(QTime(0, 0)) + (int)this);
    if (mopStepAnimation == NULL)
    {
        mopStepAnimation = new QPropertyAnimation(this, "step");
    }

    // 随机产生轨迹
    {
        int index = qrand() % mmapPath.size();
        while (!mmapPath.value(index))
        {
            index = qrand() % mmapPath.size();
        }
        QPoint curpos = pos();
        qDebug() << __FUNCTION__ << index;
        switch (index)
        {
        case 0:
            moMovePath.setLine(curpos, QPoint(qrand() % QApplication::desktop()->width(),
                                             qrand() % QApplication::desktop()->height()));
            break;
        case 1:
            moMovePath.setCurve(curpos, QPoint(qrand() % QApplication::desktop()->width(),
                                             qrand() % QApplication::desktop()->height()));
            break;
        case 2:
            moMovePath.setTrigon(curpos, QPoint(qrand() % QApplication::desktop()->width(),
                                                qrand() % QApplication::desktop()->height())
                                 , QPoint(qrand() % QApplication::desktop()->width(),
                                          qrand() % QApplication::desktop()->height()));
            break;
        case 3:
            moMovePath.setRect(curpos, QPoint(curpos.x() + qrand() % 1000 - 500,
                                              curpos.y() + qrand() % 1000 - 500));
            break;
        case 4:
            moMovePath.setCentre(curpos, QPoint(curpos.x() + qrand() % 1000 - 500,
                                                curpos.y() + qrand() % 1000 - 500));
            break;
        }
    }

    mopStepAnimation->stop();
    mopStepAnimation->setStartValue(0);
    mopStepAnimation->setEndValue(1);
    mopStepAnimation->setDuration(qrand()%5 * 1000 + 5000);
    mopStepAnimation->setEasingCurve(QEasingCurve::Type(qrand() % 40));
    mopStepAnimation->start();
}

void CSignalBarrageScreen::changeMouseThrough()
{
    if (GetWindowLong((HWND)winId(), GWL_EXSTYLE) == miWindowLong)
    {
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
                       WS_EX_TRANSPARENT | WS_EX_LAYERED);
        mbMouseThrough = true;
    }
    else
    {
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, miWindowLong);
        mbMouseThrough = false;
    }
}

void CSignalBarrageScreen::choiseMovePath()
{
    QAction *act = (QAction *)sender();

    int data = act->data().toInt();

    bool choise = mmapPath.value(data);
    mmapPath[data] = !choise;

    if (!(mmapPath[0] || mmapPath[1] || mmapPath[2] || mmapPath[3] || mmapPath[4]))
    {
        mmapPath[data] = choise;
        act->setChecked(choise);
        XYToolTips::showToolTips(QStringLiteral("至少选择一种运动轨迹！"));
    }
}

void CSignalBarrageScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter;
    painter.begin(this);
    CBarrageItem *item = mopContentsItem;
    painter.setPen(item->moTextColor);
    painter.setFont(item->moTextFont);
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
        painter.drawPixmap(QRect(0,
                                 0,
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
        {
            painter.drawText(QRect(offset_X,
                                   offset_Y,
                                   limit_X < 0 ? 0:limit_X,
                                   limit_Y < 0 ? 0:limit_Y),
                             contents->text);
            QFontMetrics metrics(item->moTextFont);
            offset_X += metrics.width(contents->text) + 2.0;
            current_Y = qMax(current_Y, metrics.height() + 2.0);
            break;
        }
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

            painter.drawPixmap(QRect(offset_X,
                                     offset_Y,
                                     qMin(limit_X < 0 ? 0:limit_X, pixmap.width()),
                                     qMin(limit_Y < 0 ? 0:limit_Y, pixmap.height())),
                               pixmap,
                               QRect(0,
                                     0,
                                     qMin(limit_X < 0 ? 0:limit_X, pixmap.width()),
                                     qMin(limit_Y < 0 ? 0:limit_Y, pixmap.height())));
            offset_X += pixmap.width();
            current_Y = qMax(current_Y, pixmap.height() + 1.0);
            break;
        }
        default:
            break;
        }

        contents = contents->next;
    }
    if (mbForceTop)
    {
        raise();
    }
    painter.end();
}

void CSignalBarrageScreen::timerEvent(QTimerEvent *event)
{
    if (miRefreshTimer == event->timerId())
    {
        update();
    }
    else if (miMoveAutoTimer == event->timerId())
    {
        if (!mbFixed && !mbLeftMousePressed)
        {
            moveNextPoint();
        }
    }
}

void CSignalBarrageScreen::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        setCurrentInstance(this);
        if (mopMoveAnimation && mopMoveAnimation->state() == QPropertyAnimation::Running)
        {
            mopMoveAnimation->stop();
        }
        if (mopStepAnimation && mopStepAnimation->state() == QPropertyAnimation::Running)
        {
            mopStepAnimation->stop();
        }
        mbLeftMousePressed = true;
        moLastPos = event->globalPos();
    }
}

void CSignalBarrageScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = false;
        moLastPos = event->globalPos();
    }
}

void CSignalBarrageScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (mbLeftMousePressed)
    {
        QPoint lastpos = pos();
        lastpos.setX( lastpos.x() + event->globalX() - moLastPos.x());
        lastpos.setY( lastpos.y() + event->globalY() - moLastPos.y());
        move(lastpos);
        moLastPos = event->globalPos();
    }
}

void CSignalBarrageScreen::contextMenuEvent(QContextMenuEvent *event)
{
    XYMenu menu(this);
    QAction *exit = new QAction("Exit", &menu);
    connect(exit, SIGNAL(triggered()), this, SLOT(close()));

    QAction *fixed = new QAction("Fixed", &menu);
    fixed->setCheckable(true);
    fixed->setChecked(mbFixed);
    connect(fixed, SIGNAL(triggered()), this, SLOT(changeFixed()));

    QAction *automove = new QAction("AutoMove", &menu);
    automove->setCheckable(true);
    automove->setChecked(mbAutoMove);
    connect(automove, SIGNAL(triggered()), this, SLOT(changeAutoMove()));

    QAction *random = new QAction("Random", &menu);
    random->setCheckable(true);
    random->setChecked(mbRandomCall);
    connect(random, SIGNAL(triggered()), this, SLOT(changeRandomCall()));

    QAction *top = new QAction("ForceTop", &menu);
    top->setCheckable(true);
    top->setChecked(mbForceTop);
    connect(top, SIGNAL(triggered()), this, SLOT(changeForceTop()));

    QAction *through = new QAction("MouseThrough", &menu);
    through->setCheckable(true);
    through->setChecked(mbMouseThrough);
    connect(through, SIGNAL(triggered()), this, SLOT(changeMouseThrough()));

    XYMenu movepath("Move Path");
    QAction *line = new QAction("Line", &movepath);
    line->setData(0);
    line->setCheckable(true);
    line->setChecked(mmapPath.value(0));
    QAction *curve = new QAction("Curve", &movepath);
    curve->setData(1);
    curve->setCheckable(true);
    curve->setChecked(mmapPath.value(1));
    QAction *trigon = new QAction("Trigon", &movepath);
    trigon->setData(2);
    trigon->setCheckable(true);
    trigon->setChecked(mmapPath.value(2));
    QAction *rect = new QAction("Rect", &movepath);
    rect->setData(3);
    rect->setCheckable(true);
    rect->setChecked(mmapPath.value(3));
    QAction *arc = new QAction("Arc", &movepath);
    arc->setData(4);
    arc->setCheckable(true);
    arc->setChecked(mmapPath.value(4));

    connect(line, SIGNAL(triggered()), this, SLOT(choiseMovePath()));
    connect(curve, SIGNAL(triggered()), this, SLOT(choiseMovePath()));
    connect(trigon, SIGNAL(triggered()), this, SLOT(choiseMovePath()));
    connect(rect, SIGNAL(triggered()), this, SLOT(choiseMovePath()));
    connect(arc, SIGNAL(triggered()), this, SLOT(choiseMovePath()));

    movepath.addAction(line);
    movepath.addAction(curve);
    movepath.addAction(trigon);
    movepath.addAction(rect);
    movepath.addAction(arc);

    menu.addMenu(&movepath);
    menu.addAction(through);
    menu.addAction(top);
    menu.addAction(random);

    menu.addAction(automove);
    menu.addAction(fixed);
    menu.addAction(exit);
    menu.exec();
}

void CSignalBarrageScreen::clicked(QPoint point)
{
    if (!mbLeftMousePressed && !mbFixed && !mbAutoMove)
    {
        raise();
        qsrand(QTime::currentTime().msecsTo(QTime(0, 0)) + (int)this);
        if (mopMoveAnimation == NULL)
        {
            mopMoveAnimation = new QPropertyAnimation(this, "pos");
        }
        else
        {
            mopMoveAnimation->stop();
        }

        mopMoveAnimation->setEndValue(point);
        mopMoveAnimation->setDuration(5000);
        mopMoveAnimation->setEasingCurve(QEasingCurve::Type(qrand() % 40));
        mopMoveAnimation->start();
    }
}
