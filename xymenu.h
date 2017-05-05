#ifndef XYMENU_H
#define XYMENU_H

#include "xybordershadowwidget.h"
#include <QEventLoop>
#include <QVBoxLayout>
#include <QIcon>
#include <QMap>

class XYMenuStyle;
class XYMenu : public XYBorderShadowWidget
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
    int exec(XYMenu *parent);
    bool close(bool closeParent = false);
    void setTitle(const QString &title);
    void setIcon(const QIcon &icon);
    void setFont(const QFont &font);
    void addMenu(XYMenu *menu);

protected:
    void childEvent(QChildEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void focusOutEvent(QFocusEvent *event);

private:
    void execMenu2(XYMenuStyle *style);
    void setupUI();

private:
    QMap<int, XYMenu *> mlistMenus;
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
