#ifndef XYMENUSTYLE_H
#define XYMENUSTYLE_H

#include <QWidget>
#include "xyaction.h"
#include "xymenu.h"

class XYMenuStyle : public QWidget
{
    Q_OBJECT
public:
    XYMenuStyle(XYAction *action, QWidget *parent = 0);
    XYMenuStyle(XYMenu *menu, QWidget *parent = 0);
    ~XYMenuStyle();

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void setMaxWidth();

private:
    static int mimaxWidth;
    XYAction  *mopAction;
    XYMenu    *mopMenu;
    bool       mbContainsMouse;

    bool       mbIsMenu;

};

#endif // XYMENUSTYLE_H
