#ifndef XYMENUSTYLE_H
#define XYMENUSTYLE_H

#include <QWidget>
#include "xymenu.h"

class XYMenuStyle : public QWidget
{
    Q_OBJECT
public:
    XYMenuStyle(QAction *action, QWidget *parent = 0);
    XYMenuStyle(XYMenu *menu, QWidget *parent = 0);
    ~XYMenuStyle();

protected:
    bool event(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QAction   *mopAction;
    XYMenu    *mopMenu;
    bool       mbContainsMouse;

    bool       mbIsMenu;

    friend class XYMenu;

};

#endif // XYMENUSTYLE_H
