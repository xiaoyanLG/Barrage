#include "xyaction.h"
#include <QDebug>
#include <QEvent>
XYAction::XYAction(QObject *parent)
    :QAction(parent)
{

}

XYAction::XYAction(const QString &title, QObject *parent)
    :QAction(title, parent)
{

}

XYAction::XYAction(const QIcon &icon, const QString &text, QObject *parent)
    :QAction(icon, text, parent)
{

}

XYAction::~XYAction()
{

}
