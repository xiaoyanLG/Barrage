#include "qexpressionlabel.h"
#include <QDebug>
#include <QMovie>
QExpressionLabel::QExpressionLabel(QWidget *parent)
    :QLabel(parent)
{

}

void QExpressionLabel::enterEvent(QEvent *event)
{
    setFrameStyle(QFrame::Box);
}

void QExpressionLabel::leaveEvent(QEvent *event)
{
    setFrameStyle(QFrame::NoFrame);
}

void QExpressionLabel::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked(movie()->fileName());
}

