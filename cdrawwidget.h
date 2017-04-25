#ifndef CDRAWWIDGET_H
#define CDRAWWIDGET_H

#include <QDialog>
#include <QMap>

class DrawWidget : public QDialog
{
    Q_OBJECT

public:
    DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

    QList<QPoint> &getDrawPoints();
    QList<qreal> &getDrawPointSpaces();
    qreal getTotalLength();
    qreal lengthToPoint(const QPoint &p1, const QPoint &p2);

public slots:
    void clearPoints();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPoint  moLastPoint;
    QPoint  moCurrentPoint;

    bool    mbPressed;
    bool    mbClear;
    QPixmap moLastPixMap;

    qreal               mfTotalLength;
    QList<QPoint>       mlistPointList;
    QList<qreal>        mlistPointSpaceList;
};

#endif // CDRAWWIDGET_H
