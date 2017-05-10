#ifndef CSIGNALBARRAGESCREEN_H
#define CSIGNALBARRAGESCREEN_H

#include "xymousemonitor.h"
#include "xybarrageitem.h"
#include "xymovepath.h"
#include <QWidget>

class XYSignalBarrageScreen : public QWidget, public XYMouseMonitor
{
    Q_OBJECT
    Q_PROPERTY(qreal step READ step WRITE setStep)
public:
    explicit XYSignalBarrageScreen(XYBarrageItem *item = NULL, QWidget *parent = 0);
    ~XYSignalBarrageScreen();
    static void deleteAllAnimations();

    qreal step() const;
    void setStep(qreal step);

public slots:
    void setItem(XYBarrageItem *item);
    void changeFixed();
    void changeForceTop();
    void changeAutoMove();
    void moveNextPoint();
    void changeMouseThrough();
    void choiseMovePath();
    void changeRandomCall();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void clicked(const QPoint &point);

private:
    void paintLineContents(QPainter &painter,
                           XYBarrageItem *item,
                           XYContents *lineContents,
                           qreal lineHeight,
                           qreal offsetX,
                           qreal offsetY);

private:
    static QList<XYSignalBarrageScreen *> allSignalScreens;
    QPropertyAnimation *mopMoveAnimation;
    QPropertyAnimation *mopStepAnimation;
    XYBarrageItem *mopContentsItem;
    XYMovePath     moMovePath;
    qreal         mfStep;

    bool          mbFixed;
    int           miRefreshTimer;
    int           miMoveAutoTimer;
    bool          mbAutoMove;
    bool          mbLeftMousePressed;
    bool          mbForceTop;
    bool          mbMouseThrough;
    bool          mbRandomCall;
    QPoint        moLastPos;
    long          miWindowLong;

    QMap<int, bool>  mmapPath;
};

#endif // CSIGNALBARRAGESCREEN_H
