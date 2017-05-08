#ifndef CSIGNALBARRAGESCREEN_H
#define CSIGNALBARRAGESCREEN_H

#include "cmousemonitorwidget.h"
#include "cbarrageitem.h"
#include "cmovepath.h"

class CSignalBarrageScreen : public CMouseMonitorWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal step READ step WRITE setStep)
public:
    explicit CSignalBarrageScreen(CBarrageItem *item = NULL, QWidget *parent = 0);
    ~CSignalBarrageScreen();

    qreal step() const;
    void setStep(qreal step);

public slots:
    void setItem(CBarrageItem *item);
    void changeFixed();
    void changeForceTop();
    void changeAutoMove();
    void moveNextPoint();
    void changeMouseThrough();
    void choiseMovePath();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

public slots:
    void clicked(QPoint point);

private:
    QPropertyAnimation *mopMoveAnimation;
    QPropertyAnimation *mopStepAnimation;
    CBarrageItem *mopContentsItem;
    CMovePath     moMovePath;
    qreal         mfStep;

    bool          mbFixed;
    int           miRefreshTimer;
    int           miMoveAutoTimer;
    bool          mbAutoMove;
    bool          mbLeftMousePressed;
    bool          mbForceTop;
    bool          mbMouseThrough;
    QPoint        moLastPos;
    long          miWindowLong;

    QMap<int, bool>  mmapPath;
};

#endif // CSIGNALBARRAGESCREEN_H
