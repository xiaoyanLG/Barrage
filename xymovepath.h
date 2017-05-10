#ifndef CMOVEPATH_H
#define CMOVEPATH_H

#include <QPoint>
#include <QRectF>
#include <QList>

struct XYPoint {
    qreal x;
    qreal y;
};

class XYShapeCircle
{
public:
    XYShapeCircle(const QPoint &centre, qreal radius);
    XYShapeCircle(const QPoint &centre, const QPoint &point);

    QList<QPoint> getPoint(qreal x, bool isX = true);

private:
    qreal mfRadius;
    QPoint moCentre;
};

class XYMovePath
{
public:
    enum TYPE {LINE, CURVE, TRIGON, RECT, ELLIPSE, ARC, UDF};
    XYMovePath(TYPE type = UDF);
    XYMovePath(const QPoint &start, const QPoint &moEnd, TYPE meType = LINE);
    XYMovePath(const QPoint &start, const QPoint &moLeftCentre, const QPoint &moRightCentre);
    void QtoCPoint(XYPoint &p1, const QPoint &p2);
    void setType(TYPE type);
    void setSetp(qreal setp);
    void setLine(const QPoint &start, const QPoint &end);
    void setCurve(const QPoint &start, const QPoint &end);
    void setTrigon(const QPoint &start, const QPoint &p1, const QPoint &p2);
    void setRect(const QPoint &start, const QPoint &rect);
    void setCentre(const QPoint &start, const QPoint &moCentre);
    void setEllipse(const QPoint &start, const QPoint &moLeftCentre, const QPoint &moRightCentre);
    ~XYMovePath();

    virtual QPoint getPoint(qreal step, bool *success = NULL); // step must between 0 and 1.

    static qreal lengthToPoint(const XYPoint &p1, const XYPoint &p2);
    static qreal lengthToPoint(const QPoint &p1, const QPoint &p2);
protected:
    qreal  mfStep;
    XYPoint moStartPoint;
    TYPE   meType;

private:
    union{
        XYPoint moEnd;    // 线段
        XYPoint moCentre; // 圆
        XYPoint moRect;   // 矩形
        struct {       // 三角形
            XYPoint moPointB;
            XYPoint moPointC;
        };
        struct {       // 椭圆
            XYPoint moLeftCentre;
            XYPoint moRightCentre;
        };
    };
};

#endif // CMOVEPATH_H
