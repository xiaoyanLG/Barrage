#ifndef XYLISTMODEL_H
#define XYLISTMODEL_H

#include <QObject>
#include "xytranslateitem.h"

class XYTranslateModel : public QObject
{
    Q_OBJECT
public:
    enum TYPE {SOURCE, TRANSLATE, COMPLETE, COMP_TRAN, TRAN_COMP, EXTRA, TIMES};
    explicit XYTranslateModel(QObject *parent = 0);
    ~XYTranslateModel();
    void appendData(const QList<XYTranslateItem *> &list);
    void prependData(const QList<XYTranslateItem *> &list);
    void setData(const QList<XYTranslateItem *> &list, bool del = true);
    void delItem(XYTranslateItem *item, bool del = true);
    XYTranslateItem *stickItem(XYTranslateItem *item);
    XYTranslateItem *getItem(int index);
    void clear(bool del = false);

    bool haveData(int index) const;
    int  counts();
    QString data(int index, int role = SOURCE) const;

private:
    QList<XYTranslateItem *> mlistFindTranslates;

};

#endif // XYLISTMODEL_H
