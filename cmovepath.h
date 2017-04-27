#ifndef CMOVEPATH_H
#define CMOVEPATH_H

#include <QPoint>
#include <QRectF>

struct CPoint {
    qreal x;
    qreal y;
};

class CShapeCircle
{
public:
    CShapeCircle(const QPoint &centre, qreal radius);
    CShapeCircle(const QPoint &centre, const QPoint &point);

    QList<QPoint> getPoint(qreal x, bool isX = true);

private:
    qreal mfRadius;
    QPoint moCentre;
};

class CMovePath
{
public:
    enum TYPE {LINE, CURVE, TRIGON, RECT, ELLIPSE, ARC, UDF};
    CMovePath(TYPE type = UDF);
    CMovePath(const QPoint &start, const QPoint &moEnd, TYPE meType = LINE);
    CMovePath(const QPoint &start, const QPoint &moLeftCentre, const QPoint &moRightCentre);
    void QtoCPoint(CPoint &p1, const QPoint &p2);
    void setType(TYPE type);
    void setSetp(qreal setp);
    void setLine(const QPoint &start, const QPoint &end);
    void setCurve(const QPoint &start, const QPoint &end);
    void setTrigon(const QPoint &start, const QPoint &p1, const QPoint &p2);
    void setRect(const QPoint &start, const QPoint &rect);
    void setCentre(const QPoint &start, const QPoint &moCentre);
    void setEllipse(const QPoint &start, const QPoint &moLeftCentre, const QPoint &moRightCentre);
    ~CMovePath();

    virtual QPoint getPoint(qreal step, bool *success = NULL); // step must between 0 and 1.

    static qreal lengthToPoint(const CPoint &p1, const CPoint &p2);
    static qreal lengthToPoint(const QPoint &p1, const QPoint &p2);
protected:
    qreal  mfStep;
    CPoint moStartPoint;
    TYPE   meType;

private:
    union{
        CPoint moEnd;    // 线段
        CPoint moCentre; // 圆
        CPoint moRect;   // 矩形
        struct {       // 三角形
            CPoint moPointB;
            CPoint moPointC;
        };
        struct {       // 椭圆
            CPoint moLeftCentre;
            CPoint moRightCentre;
        };
    };
};

#endif // CMOVEPATH_H
