#ifndef XYTOOLTIPS_H
#define XYTOOLTIPS_H

#include "xybordershadowwidget.h"
#include "xymousemonitor.h"
#include <QTimer>

class XYToolTips : public XYBorderShadowWidget, public XYMouseMonitor
{
    Q_OBJECT
public:
    ~XYToolTips();
    static XYToolTips *getInstance();
    static void showToolTips(const QString &tooltips, QWidget *parent = NULL);
    static void showToolTips(const QString &tooltips, const QPoint &pos, QWidget *parent = NULL);

public slots:
    bool close();

protected:
    void paintEvent(QPaintEvent *event);
    void clicked(const QPoint &point);
    void timerEvent(QTimerEvent *event);

private:
    explicit XYToolTips(QWidget *parent = 0);
    static XYToolTips* mopInstance;

    int      miMonitorTimer;
    QWidget *mopParent;
    QTimer   moTimer;
    QString  msToolTips;
};

#endif // XYTOOLTIPS_H
