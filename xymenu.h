#ifndef XYMENU_H
#define XYMENU_H

#include "xybordershadowwidget.h"
#include "xymousemonitor.h"
#include <QIcon>
#include <QMap>

class XYMenuStyle;
class QEventLoop;
class QVBoxLayout;

class XYMenu : public XYBorderShadowWidget, public XYMouseMonitor
{
    Q_OBJECT
public:
    explicit XYMenu(QWidget *parent = 0);
    XYMenu(const QString &title, QWidget *parent = 0);
    XYMenu(const QIcon &icon, const QString &title, QWidget *parent = 0);
    QString title();
    QIcon icon();
    QFont font();
    ~XYMenu();

public slots:
    int exec();
    void raise();
    void updateCheckedMenu();
    int exec(XYMenu *parent);
    bool close(bool closeParent = false);
    void setTitle(const QString &title);
    void setIcon(const QIcon &icon);
    void setFont(const QFont &font);
    void addMenu(XYMenu *menu);

protected:
    void focusOutEvent(QFocusEvent *event);
    void leaveEvent(QEvent *event);
    void clicked(const QPoint &point);

private:
    void execMenu2(XYMenuStyle *style);
    void setupUI();

private:
    static XYMenu      *mopLastMenu;
    QMap<int, XYMenu *> mlistMenus;
    QList<QWidget *>    mlistWidgets;
    XYMenu        *mopParentMenu;
    QEventLoop    *mopEventLoop;
    QVBoxLayout   *mopMainLayout;
    XYMenuStyle   *mopCurrentChecked;

    QString        msTitle;
    QIcon          moIcon;
    QFont          moFont;
    int            miActionMaxWidth;

    friend class   XYMenuStyle;
};

#endif // XYMENU_H
