#ifndef XYTRANSLATEVIEW_H
#define XYTRANSLATEVIEW_H

#include "xybordershadowwidget.h"
#include "xytranslatemodel.h"

class XYMenu;
class XYTranslateDelegate;
class XYTranslateView : public XYBorderShadowWidget
{
    Q_OBJECT
public:
    explicit XYTranslateView(QWidget *parent = 0);
    ~XYTranslateView();
    void appendData(const QList<XYTranslateItem *> &list);
    void prependData(const QList<XYTranslateItem *> &list);
    void setData(const QList<XYTranslateItem *> &list, bool del = true);
    void clear(bool del = true);
    void setFont(const QFont &font);
    void nextPage();
    void prePage();
    int  itemCount();
    XYTranslateItem *getItem(int index);
    QString getData(int index);    // 通过数字键选择输入

public slots:
    void show();
    void close();
    void stickItem();
    void delItem();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    XYTranslateItem *contentsItem(const QPoint &point);
    int getItemIndex(const QPoint &point);

private:
    XYTranslateModel *mopModel;
    XYMenu *mopMenu;
    QFont moFont;

    int miCurrentPage;
    int miMaxVisibleItem;
    XYTranslateModel::TYPE showType;

    struct
    {
        QRect rect;
        XYTranslateItem *item;
    } mlistCurrentItems[9];         // 保存当前显示的内容，并记录位置信息

    friend class XYInput;
};

#endif // XYTRANSLATEVIEW_H
