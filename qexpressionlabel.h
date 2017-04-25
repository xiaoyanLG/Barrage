#ifndef QEXPRESSIONLABEL_H
#define QEXPRESSIONLABEL_H

#include <QLabel>

class QExpressionLabel : public QLabel
{
    Q_OBJECT
public:
    QExpressionLabel(QWidget *parent = NULL);

signals:
    void clicked(const QString &filename);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // QEXPRESSIONLABEL_H
