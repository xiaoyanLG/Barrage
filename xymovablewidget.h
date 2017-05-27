#ifndef XYMOVABLEWIDGET_H
#define XYMOVABLEWIDGET_H

#include <QWidget>

class XYMovableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XYMovableWidget(QWidget *parent = 0);
    ~XYMovableWidget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    bool          mbLeftMousePressed;
    QPoint        moLastPos;

};

#endif // XYMOVABLEWIDGET_H
