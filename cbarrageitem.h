#ifndef CBARRAGEITEM_H
#define CBARRAGEITEM_H
#include <QObject>
#include <QPoint>
#include <QColor>
#include <QFont>
#include <QMovie>
#include <QPropertyAnimation>
#include <QTimerEvent>

class Contents
{
public:
    enum {NONE, TEXT, LF, PIXMAP};

    Contents()
    {
        type = NONE;
        next = NULL;
    }
    Contents(const QString &text)
    {
        type = TEXT;
        this->text = text;
        next = NULL;
    }
    Contents(const QMovie &pixmap)
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

    Contents *next;
};

class CBarrageItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint pos READ getPos WRITE setPos)
    Q_PROPERTY(qreal opactiy READ getOpactiy WRITE setOpactiy)
public:
    explicit CBarrageItem(Contents *contents = NULL,
                     int   showTimes = 8000,
                     const QColor &textColor = QColor("green"),
                     const QFont  &textFont = QFont("黑体", 25),
                     int barrageWidth = -1);
    ~CBarrageItem();

    QPoint getCurrentPos();
    qreal getCurrentOpacity();

    // 使用特殊运动轨迹
    void setAnimation(const QEasingCurve &type);

    QPoint getPos() const;
    void setPos(const QPoint &pos);

    qreal getOpactiy() const;
    void setOpactiy(qreal opactiy);

    bool isFinished();

private:
    qreal lengthToPoint(const QPoint &p1, const QPoint &p2);

public slots:
    void setShowTimes(int showTimes, int counts = 1);
    void setContents(Contents *contents);
    void setTextColor(const QColor &textColor);
    void setTextFont(const QFont  &textFont);
    void setStartPos(const QPoint &startPos);
    void setEndPos(const QPoint &endPos);
    void setBarrageSize(int barrageWidth, int barrageHeight);
    void setDrawPoints(const QList<QPoint> &points);
    void setBackImage(const QMovie &image);

private:
    QPropertyAnimation *mopMoveAnimation;
    QPropertyAnimation *mopOpactiyAnimation;
    qreal   mfOpactiy;
    bool    mbStarted;
    bool    mbUseUserDrawPoints;
    QPoint  moStartPos;
    QPoint  moEndPos;
    QPoint  moCurrentPos;
    int     miShowTimes;
    int     miLoopCounts;
    QString msContents;
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

    Contents     *mopContents;
    friend class CBarrageScreen;
    friend class CSignalBarrageScreen;
};

#endif // CBARRAGEITEM_H
