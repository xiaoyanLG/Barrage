#ifndef XYMENU_H
#define XYMENU_H

#include "xybordershadowwidget.h"
#include <QEventLoop>
#include <QVBoxLayout>

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
    void childEvent(QChildEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void focusOutEvent(QFocusEvent *event);

private:
    QEventLoop  *mopEventLoop;
    QVBoxLayout *mopMainLayout;
};

#endif // XYMENU_H
