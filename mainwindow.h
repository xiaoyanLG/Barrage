#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMap>
#include <QMovie>
namespace Ui {
class MainWindow;
}

class CBarrageItem;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    CBarrageItem *getItem(int time = 0);

public slots:
    void addItem();
    void addItem(int time);
    void getPoints();
    void setBackImage();
    void insertImage(const QString &filename);
    void insertImage();
    void timerShot(int number);
    void addAnimation();

protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::MainWindow *ui;
    int                  miTimer;
    int                  miLoopShot;

    QMovie               moBackImage;
    qreal                mfTotalLength;
    QList<QPoint>        mlistDrawPoints;
    QList<qreal>         mlistPointSpaces;
};

#endif // MAINWINDOW_H
