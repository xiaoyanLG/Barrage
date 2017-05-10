#ifndef CEMOTICONWIDGET_H
#define CEMOTICONWIDGET_H

#include <QDialog>

class XYEmoticonWidget : public QDialog
{
    Q_OBJECT
public:
    explicit XYEmoticonWidget(QWidget *parent = 0);

public slots:
    void openImage();

signals:
    void clicked(const QString &filename);

};

#endif // CEMOTICONWIDGET_H
