#ifndef XYMENUSTYLE_H
#define XYMENUSTYLE_H

#include <QWidget>
#include "xyaction.h"

class XYMenuStyle : public QWidget
{
    Q_OBJECT
public:
    explicit XYMenuStyle(QWidget *parent = 0);
    XYMenuStyle(XYAction *action, QWidget *parent = 0);
    ~XYMenuStyle();

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    static int mimaxWidth;
    XYAction  *mopAction;
    bool       mbContainsMouse;

};

#endif // XYMENUSTYLE_H
