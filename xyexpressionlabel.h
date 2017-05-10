#ifndef QEXPRESSIONLABEL_H
#define QEXPRESSIONLABEL_H

#include <QLabel>

class XYExpressionLabel : public QLabel
{
    Q_OBJECT
public:
    XYExpressionLabel(QWidget *parent = NULL);

signals:
    void clicked(const QString &filename);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // QEXPRESSIONLABEL_H
