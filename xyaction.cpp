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

XYAction::~XYAction()
{

}

bool XYAction::event(QEvent *event)
{
    qDebug() << "+++++++++++++++6666666666666666666";
    event->accept();
    return false;
}

