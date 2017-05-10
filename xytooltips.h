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
    static void showToolTips(const QString &tooltips);
    static void showToolTips(const QString &tooltips, const QPoint &pos);

public slots:
    bool close();

protected:
    void paintEvent(QPaintEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void clicked(const QPoint &point);

private:
    explicit XYToolTips(QWidget *parent = 0);
    static XYToolTips* mopInstance;

    QTimer  moTimer;
    QString msToolTips;
};

#endif // XYTOOLTIPS_H
