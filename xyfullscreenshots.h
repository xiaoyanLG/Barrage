#ifndef XYFULLSCREEN_H
#define XYFULLSCREEN_H

#include <QWidget>

class XYFullScreenShots : public QWidget
{
    Q_OBJECT
public:
    static XYFullScreenShots *getInstance();
    ~XYFullScreenShots();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    //处理全局热键
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    explicit XYFullScreenShots(QWidget *parent = 0);
    void ShowMenu();

private slots:
    void startScreenShots();
    void endScreenShots();
    void saveChoisePixmap();
    void resetChoiseRectSize();
    void saveToClipboard();

private:
    static XYFullScreenShots *mopInstance;
    QRect    moCurrentRect;
    QPixmap  moBackPixmap;
    QPixmap  moChoiseRectPixmap;
    QPoint   moPressPoint;
    QPoint   moDrawPoint;
    bool     mbLeftPress;
    bool     mbMoveRect;

};

#endif // XYFULLSCREEN_H
