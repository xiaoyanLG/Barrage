#include "cmovepath.h"
#include <QtMath>
#include <QDebug>

CShapeCircle::CShapeCircle(const QPoint &centre, qreal radius)
{
    moCentre = centre;
    mfRadius = radius;
}

CShapeCircle::CShapeCircle(const QPoint &centre, const QPoint &point)
{
    moCentre = centre;
    mfRadius = CMovePath::lengthToPoint(centre, point);
}

QList<QPoint> CShapeCircle::getPoint(qreal x, bool isX)
{
    qreal other = 0;
    int length = 0;
    int ab = 0;
    if (isX)
    {
        length = x - moCentre.x();
        ab = moCentre.y();
    }
    else
    {
        length = x - moCentre.y();
        ab = moCentre.x();
    }
    other = qSqrt(mfRadius * mfRadius - length * length);

    if (isX)
    {
        return QList<QPoint>() << QPoint(x, other + ab) << QPoint(x, -other + ab);
    }
    else
    {
        return QList<QPoint>() << QPoint(other + ab, x) << QPoint(-other + ab, x);
    }
}

CMovePath::CMovePath(CMovePath::TYPE type)
{
    this->meType = type;
    this->mfStep = 0;
}

CMovePath::CMovePath(const QPoint &start, const QPoint &end, CMovePath::TYPE type)
{
    QtoCPoint(this->moStartPoint, start);
    switch (type)
    {
    case LINE:
        QtoCPoint(this->moEnd, end);
        break;
    case RECT:
        QtoCPoint(this->moRect, end);
        break;
    case ARC:
        QtoCPoint(this->moCentre, end);
        break;
    default:
        QtoCPoint(this->moEnd, end);
        break;
    }
    this->meType = type;
    this->mfStep = 1;
}

CMovePath::CMovePath(const QPoint &start, const QPoint &leftCentre, const QPoint &rightCentre)
{
    QtoCPoint(this->moStartPoint, start);
    QtoCPoint(this->moLeftCentre, leftCentre);
    QtoCPoint(this->moRightCentre, rightCentre);
    this->meType = ELLIPSE;
    this->mfStep = 1;
}

void CMovePath::QtoCPoint(CPoint &p1, const QPoint &p2)
{
    p1.x = p2.x();
    p1.y = p2.y();
}

void CMovePath::setType(CMovePath::TYPE type)
{
    this->meType = type;
}

void CMovePath::setSetp(qreal setp)
{
    this->mfStep = setp;
}

void CMovePath::setLine(const QPoint &start, const QPoint &end)
{
    QtoCPoint(this->moStartPoint, start);
    QtoCPoint(this->moEnd, end);
    this->meType = LINE;
    this->mfStep = 1;
}

void CMovePath::setCurve(const QPoint &start, const QPoint &end)
{
    QtoCPoint(this->moStartPoint, start);
    QtoCPoint(this->moEnd, end);
    this->meType = CURVE;
    this->mfStep = 1;
}

void CMovePath::setTrigon(const QPoint &start, const QPoint &p1, const QPoint &p2)
{
    QtoCPoint(this->moStartPoint, start);
    QtoCPoint(this->moPointB, p1);
    QtoCPoint(this->moPointC, p2);
    this->meType = TRIGON;
    this->mfStep = 1;
}

void CMovePath::setRect(const QPoint &start, const QPoint &rect)
{
    QtoCPoint(this->moStartPoint, start);
    QtoCPoint(this->moRect, rect);
    this->meType = RECT;
    this->mfStep = 1;
}

void CMovePath::setCentre(const QPoint &start, const QPoint &centre)
{
    QtoCPoint(this->moStartPoint, start);
    QtoCPoint(this->moCentre, centre);
    this->meType = ARC;
    this->mfStep = 1;
}

void CMovePath::setEllipse(const QPoint &start, const QPoint &leftCentre, const QPoint &rightCentre)
{
    QtoCPoint(this->moStartPoint, start);
    QtoCPoint(this->moLeftCentre, leftCentre);
    QtoCPoint(this->moRightCentre, rightCentre);
    this->meType = ELLIPSE;
    this->mfStep = 1;
}

CMovePath::~CMovePath()
{

}

QPoint CMovePath::getPoint(qreal step, bool *success)
{
    if (step < 0 || step > 1)
    {
        if (success)
        {
            *success = false;
            return QPoint();
        }
    }
    else
    {
        if (success)
        {
            *success = true;
        }
    }
    qreal x = 0;
    qreal y = 0;
    step *= mfStep;
    switch (meType)
    {
    case LINE: // 直线
        x = (moEnd.x - moStartPoint.x) * step;
        y = (moEnd.y - moStartPoint.y) * step;
        x += moStartPoint.x;
        y += moStartPoint.y;
        break;
    case CURVE:    // 默认实现(物理)抛物线，可以重载实现其他曲线
    {
        qreal height = moEnd.y - moStartPoint.y;
        qreal width  = moEnd.x - moStartPoint.x;
        qreal g = width * width / height;
        x = (moEnd.x - moStartPoint.x) * step;
        y = x * x / g;
        x += moStartPoint.x;
        y += moStartPoint.y;
        break;
    }
    case TRIGON:    // 三角形
    {
        qreal AB = lengthToPoint(moStartPoint, moPointB);
        qreal BC = lengthToPoint(moPointB, moPointC);
        qreal CA = lengthToPoint(moPointC, moStartPoint);
        qreal perimeter = AB + BC + CA;
        qreal length = step * perimeter;
        // 运动轨迹ABC
        if (length <= AB)
        {
            x = (moPointB.x - moStartPoint.x) * step / AB * perimeter;
            y = (moPointB.y - moStartPoint.y) * step / AB * perimeter;
            x += moStartPoint.x;
            y += moStartPoint.y;
        }
        else if (length <= AB + BC)
        {
            x = (moPointC.x - moPointB.x) * (step - AB / perimeter) / BC * perimeter;
            y = (moPointC.y - moPointB.y) * (step - AB / perimeter) / BC * perimeter;
            x += moPointB.x;
            y += moPointB.y;
        }
        else
        {
            x = (moStartPoint.x - moPointC.x) * (step - (AB + BC) / perimeter) / CA * perimeter;
            y = (moStartPoint.y - moPointC.y) * (step - (AB + BC) / perimeter) / CA * perimeter;
            x += moPointC.x;
            y += moPointC.y;
        }
        break;
    }
    case RECT: // 矩形
    {
        CPoint A = moStartPoint;
        CPoint B = moEnd;
        CPoint C = CPoint{B.x, A.y};
        CPoint D = CPoint{A.x, B.y};

        qreal AC = lengthToPoint(A, C);
        qreal AD = lengthToPoint(A, D);
        qreal CB = lengthToPoint(C, B);
        qreal DB = lengthToPoint(B, D);
        qreal perimeter = AC + AD + CB + DB;
        qreal length = step * perimeter;

        // 运动轨迹ACBD
        if (length <= AC)
        {
            y = A.y;
            x = A.x + (C.x - A.x) * step / AC * perimeter;
        }
        else if (length <= AC + CB)
        {
            x = C.x;
            y = C.y + (B.y - C.y) * (step - AC / perimeter) / CB * perimeter;
        }
        else if (length <= AC + CB + DB)
        {
            y = B.y;
            x = B.x + (A.x - C.x) * (step - 0.5) / DB * perimeter;
        }
        else
        {
            x = D.x;
            y = D.y + (A.y - D.y) * (step - 0.5 - DB / perimeter) / AD * perimeter;
        }
        break;
    }
    case ELLIPSE: // 还未完整实现
    {
        qreal a = lengthToPoint(moLeftCentre, moRightCentre);
        qreal b = lengthToPoint(moLeftCentre, moStartPoint);
        qreal c = lengthToPoint(moRightCentre, moStartPoint);
        qreal L = b + c;
#define COS(a, b, c) (((a)*(a) + (b)*(b) - (c)*(c))/2/(a)/(b))

        qreal C = COS(a, b, c);
        qreal m = step * mfStep * 360;

        qreal xl = (a*a - L*L) / (qCos(C)*a - 2*L);


        x = (qSin(C-m) * xl) / qSin(C) / b * moStartPoint.x;
        y = (qCos(C-m) * xl) / qCos(C) / b * moStartPoint.y;
        break;
    }
    case ARC: // 圆形
    {
        // 圆半径
        qreal radius = lengthToPoint(moCentre, moStartPoint);
        int bL = moCentre.x > moStartPoint.x ? -1:1;
        // 起始位置斜率
        if (moCentre.x == moStartPoint.x)
        {
            moStartPoint.x += 1;
        }

        qreal startAngle = qAtan((moCentre.y - moStartPoint.y)/(moCentre.x - moStartPoint.x)) * 180 / M_PI;

        // 旋转角度
        qreal angleA = step * mfStep * 360;
        // 半径的平方
        qreal radius2 = radius * radius;

        // 旋转后直线斜率
        qreal k;
        // 旋转后和圆的交点坐标（两个交点）
        qreal x1;
        qreal x2;
        qreal y1;
        qreal y2;
        if (qAbs(startAngle + angleA) != 90 || qAbs(startAngle + angleA) != 270)
        {
            k = qTan((startAngle + angleA) / 180 * M_PI);
            x1 = bL * qSqrt(radius2 / (1 + k * k)) + moCentre.x;
            x2 = bL * -qSqrt(radius2 / (1 + k * k)) + moCentre.x;

            y1 = k * (x1 - moCentre.x) + moCentre.y;
            y2 = k * (x2 - moCentre.x) + moCentre.y;
        }
        else
        {
            x1 = x2 = moCentre.x;
            y1 = moCentre.y + radius;
            y2 = moCentre.y - radius;
        }

        if (startAngle + angleA <= 90)
        {
            x = x1;
            y = y1;
        }
        else if (startAngle + angleA <= 180)
        {
            x = x2;
            y = y2;
        }
        else if (startAngle + angleA <= 270)
        {
            x = x2;
            y = y2;
        }
        else
        {
            x = x1;
            y = y1;
        }
        break;
    }
    default:
        break;
    }
    return QPoint(x, y);
}

qreal CMovePath::lengthToPoint(const CPoint &p1, const CPoint &p2)
{
    qreal x = qAbs(p1.x - p2.x);
    qreal y = qAbs(p1.y - p2.y);
    return qSqrt((x*x) + (y*y));
}

qreal CMovePath::lengthToPoint(const QPoint &p1, const QPoint &p2)
{
    qreal x = qAbs(p1.x() - p2.x());
    qreal y = qAbs(p1.y() - p2.y());
    return qSqrt((x*x) + (y*y));
}
