#ifndef CBARRAGESCREEN_H
#define CBARRAGESCREEN_H

#include <QWidget>
#include "cbarrageitem.h"
#include <QList>

class CBarrageScreen : public QWidget
{
    Q_OBJECT
public:
    ~CBarrageScreen();
    static CBarrageScreen *getScreen();

public slots:
    void addItem(CBarrageItem *item);
    void delItem(CBarrageItem *item);
    void setMaxBarrageNumber(int max);

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    explicit CBarrageScreen(QWidget *parent = 0);

private:
    QList<CBarrageItem *> mlistBarrageItems;
    static CBarrageScreen *mopInstance;

    int                   miMaxBarrageNumber;
    int                   miRefreshTimer;
};

#endif // CBARRAGESCREEN_H
