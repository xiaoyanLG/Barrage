#ifndef BARRAGE_H
#define BARRAGE_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>

class QPropertyAnimation;
class Barrage : public QWidget
{
    Q_OBJECT
public:
    explicit Barrage(const QString &contents,
                     int   showTimes = 8000,
                     const QColor &textColor = QColor("green"),
                     const QFont  &textFont = QFont("黑体", 25),
                     const QPoint &startPos = QPoint(QApplication::desktop()->width(), 50),
                     int barrageWidth = -1);
    ~Barrage();

protected:
    void paintEvent(QPaintEvent * event);

public slots:
    void setShowTimes(int showTimes);
    void setTextColor(const QColor &textColor);
    void setTextFont(const QFont  &textFont);
    void setStartPos(const QPoint &startPos);
    void setBarrageWidth(int barrageWidth);
    void show();

private:
    QPropertyAnimation *mopMoveAnimation;
    QPoint  moStartPos;
    int     miShowTimes;
    QString msContents;
    QColor  moTextColor;
    QFont   moTextFont;
    int     miBarrageWidth;
};

#endif // BARRAGE_H
