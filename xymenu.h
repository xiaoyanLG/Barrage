#ifndef XYMENU_H
#define XYMENU_H

#include "xybordershadowwidget.h"
#include <QEventLoop>

class XYMenu : public XYBorderShadowWidget
{
    Q_OBJECT
public:
    explicit XYMenu(QWidget *parent = 0);
    ~XYMenu();

public slots:
    int exec();
    bool close();

protected:
    void focusOutEvent(QFocusEvent *event);

private:
    QEventLoop *mopEventLoop;
};

#endif // XYMENU_H
