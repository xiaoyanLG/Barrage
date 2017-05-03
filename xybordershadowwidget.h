#ifndef XYBORDERSHADOWWIDGET_H
#define XYBORDERSHADOWWIDGET_H

#include <QWidget>

class XYBorderShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XYBorderShadowWidget(QWidget *parent = 0);
    ~XYBorderShadowWidget();

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // XYBORDERSHADOWWIDGET_H
