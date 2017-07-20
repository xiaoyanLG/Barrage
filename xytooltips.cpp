#include "xytooltips.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QTimerEvent>

XYToolTips *XYToolTips::mopInstance = NULL;
XYToolTips::XYToolTips(QWidget *parent)
    : XYBorderShadowWidget(parent), XYMouseMonitor()
{
    this->setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool
                   | Qt::WindowDoesNotAcceptFocus);

    mopParent = NULL;
    miMonitorTimer = startTimer(50);
    moTimer.setInterval(5000);
    moTimer.setSingleShot(true);
    connect(&moTimer, SIGNAL(timeout()), this, SLOT(close()));
}

XYToolTips::~XYToolTips()
{

}

XYToolTips *XYToolTips::getInstance()
{
    if (mopInstance == NULL)
    {
        mopInstance = new XYToolTips;
    }
    return mopInstance;
}

void XYToolTips::showToolTips(const QString &tooltips, QWidget *parent)
{
    QPoint pos = QCursor::pos();
    showToolTips(tooltips, pos, parent);
}

void XYToolTips::showToolTips(const QString &tooltips, const QPoint &pos, QWidget *parent)
{
    if (tooltips.trimmed().isEmpty())
    {
        return;
    }
    XYToolTips *toolwd = getInstance();
    if (parent)
    {
        toolwd->mopParent = parent;
    }
    if (!toolwd->isHidden())
    {
        toolwd->close();
    }
    toolwd->msToolTips = tooltips;
    QFontMetrics metrics(qApp->font());
    int w = metrics.width(tooltips) + 25;
    int h = metrics.height() + 25;
    toolwd->show();

    QDesktopWidget *top = QApplication::desktop();
    QPoint f_pos = pos;
    if (top->width() < w + f_pos.x())
    {
        f_pos.setX(top->width() - w - 5);
    }
    if (top->height() < h + f_pos.y())
    {
        f_pos.setY(top->height() - h - 5);
    }
    toolwd->move(f_pos);
    toolwd->moTimer.start();
}

bool XYToolTips::close()
{
    mopParent = NULL;
    moTimer.stop();
    return QWidget::close();
}

void XYToolTips::paintEvent(QPaintEvent *event)
{
    QFontMetrics metrics(qApp->font());
    int w = metrics.width(msToolTips);
    int h = metrics.height();

    this->setFixedSize(w + 30, h + 30);
    XYBorderShadowWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawText(QRect( (width() - w) / 2,
                           (height() - h) / 2,
                           width(),
                           height()),
                     msToolTips);
}

void XYToolTips::clicked(const QPoint &point)
{
    close();
}

void XYToolTips::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == miMonitorTimer)
    {
        if (mopParent && !mopParent->isVisible())
        {
            close();
        }
    }
}

