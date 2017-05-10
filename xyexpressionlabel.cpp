#include "xyexpressionlabel.h"
#include <QMovie>

XYExpressionLabel::XYExpressionLabel(QWidget *parent)
    :QLabel(parent)
{

}

void XYExpressionLabel::enterEvent(QEvent *event)
{
    setFrameStyle(QFrame::Box);
}

void XYExpressionLabel::leaveEvent(QEvent *event)
{
    setFrameStyle(QFrame::NoFrame);
}

void XYExpressionLabel::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked(movie()->fileName());
}

