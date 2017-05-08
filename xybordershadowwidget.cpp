#include "xybordershadowwidget.h"
#include <QPainter>
#include <QtMath>

XYBorderShadowWidget::XYBorderShadowWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
}

XYBorderShadowWidget::~XYBorderShadowWidget()
{

}

void XYBorderShadowWidget::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width()-20, this->height()-20);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0,0,0,0);
    for (int i=0; i<10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2, i, i);
        int alpha = 150 - qSqrt(i)*50;
        color.setAlpha(alpha > 0? alpha/3:0);
        painter.setPen(color);
        painter.drawPath(path);
    }
}
