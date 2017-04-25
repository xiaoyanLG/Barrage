#ifndef CEMOTICONWIDGET_H
#define CEMOTICONWIDGET_H

#include <QDialog>

class CEmoticonWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CEmoticonWidget(QWidget *parent = 0);

public slots:
    void openImage();

signals:
    void clicked(const QString &filename);

};

#endif // CEMOTICONWIDGET_H
