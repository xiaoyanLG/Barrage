#ifndef XYACTION_H
#define XYACTION_H

#include <QAction>

class XYAction : public QAction
{
    Q_OBJECT
public:
    XYAction(QObject *parent = NULL);
    XYAction(const QString &title, QObject *parent = NULL);
    ~XYAction();

protected:
    bool event(QEvent *event);
};

#endif // XYACTION_H
