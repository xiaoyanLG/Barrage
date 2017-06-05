#ifndef CBARRAGEITEM_H
#define CBARRAGEITEM_H
#include <QObject>
#include <QPoint>
#include <QColor>
#include <QFont>
#include <QMovie>
#include <QPropertyAnimation>
#include <QTimerEvent>

class XYContents
{
public:
    enum TYPE{NONE, TEXT, LF, PIXMAP};

    XYContents()
    {
        type = NONE;
        next = NULL;
    }
    XYContents(TYPE t)
    {
        type = t;
        next = NULL;
    }
    XYContents(const QString &text)
    {
        type = TEXT;
        this->text = text;
        next = NULL;
    }
    XYContents(const QMovie &pixmap)
    {
        type = PIXMAP;
        if (pixmap.isValid())
        {
            this->pixmap.setFileName(pixmap.fileName());
            this->pixmap.start();
        }
        next = NULL;
    }

    int type;

    QString text;
    QMovie  pixmap;

    XYContents *next;
};

class XYBarrageItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint pos READ getPos WRITE setPos)
    Q_PROPERTY(qreal opactiy READ getOpactiy WRITE setOpactiy)
public:
    explicit XYBarrageItem(XYContents *contents = NULL,
                     int   showTimes = 8000,
                     const QColor &textColor = QColor("green"),
                     const QFont  &textFont = QFont("黑体", 25),
                     int barrageWidth = -1);
    ~XYBarrageItem();

    QPoint getCurrentPos(bool start = true);
    qreal getCurrentOpacity();

    int getContentsWidth();
    int getContentsHeight();

    int getRealContentsWidth();
    int getRealContentsHeight();

    bool isAdjust();
    void setAdjust(bool adjust);

    void offsetY(int offset); // 平移，试图让弹幕不覆盖

    QPoint getPos() const;
    void setPos(const QPoint &pos);

    qreal getOpactiy() const;
    void setOpactiy(qreal opactiy);

    // 使用特殊运动轨迹
    void setAnimation(const QEasingCurve &type);

    bool isFinished();

private:
    qreal lengthToPoint(const QPoint &p1, const QPoint &p2);
    void deleteContents();

public slots:
    void setShowTimes(int showTimes, int counts = 1);
    void setContents(XYContents *contents);
    void setTextColor(const QColor &textColor);
    void setTextFont(const QFont  &textFont);
    void setStartPos(const QPoint &startPos);
    void setEndPos(const QPoint &endPos);
    void setBarrageSize(int barrageWidth, int barrageHeight);
    void setDrawPoints(const QList<QPoint> &points);
    void setBackImage(const QMovie &image);

public:
    QPropertyAnimation *mopMoveAnimation;
    QPropertyAnimation *mopOpactiyAnimation;
    qreal   mfOpactiy;
    bool    mbAdjust;
    bool    mbStarted;
    bool    mbUseUserDrawPoints;
    QPoint  moStartPos;
    QPoint  moEndPos;
    QPoint  moCurrentPos;
    int     miShowTimes;
    int     miLoopCounts;
    QColor  moTextColor;
    QFont   moTextFont;
    int     miBarrageWidth;
    int     miBarrageHeight;

    qreal   mfSetpX;
    qreal   mfSetpY;

    qreal   mfCurrentX;
    qreal   mfCurrentY;

    bool    mbEqualX;
    bool    mbEqualY;

    QList<QPoint> mlistDrawPoints;
    QList<qreal>  mlistPointSpaces;
    qreal         mfTotalLength;

    QMovie        moBackImage;

    XYContents     *mopContents;

    friend class XYBarrageScreen;
    friend class XYSignalBarrageScreen;
};

#endif // CBARRAGEITEM_H
