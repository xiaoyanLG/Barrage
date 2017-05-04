#ifndef XYACTION_H
#define XYACTION_H

#include <QAction>

class XYAction : public QAction
{
    Q_OBJECT
public:
    XYAction(QObject *parent = NULL);
    XYAction(const QString &title, QObject *parent = NULL);
    XYAction(const QIcon &icon, const QString &text, QObject *parent = NULL);
    ~XYAction();

};

#endif // XYACTION_H
