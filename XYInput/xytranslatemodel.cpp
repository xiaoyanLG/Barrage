#include "xytranslatemodel.h"

XYTranslateModel::XYTranslateModel(QObject *parent)
    :QObject(parent)
{

}

XYTranslateModel::~XYTranslateModel()
{

}

void XYTranslateModel::appendData(const QList<XYTranslateItem *> &list)
{
    mlistFindTranslates += list;
}

void XYTranslateModel::prependData(const QList<XYTranslateItem *> &list)
{
    mlistFindTranslates = list + mlistFindTranslates;
}

void XYTranslateModel::setData(const QList<XYTranslateItem *> &list, bool del)
{
    clear(del);
    mlistFindTranslates = list;
}

void XYTranslateModel::delItem(XYTranslateItem *item, bool del)
{
    if (del)
    {
        delete item;
    }
    mlistFindTranslates.removeAll(item);
}

XYTranslateItem * XYTranslateModel::stickItem(XYTranslateItem *item)
{
    XYTranslateItem *lastStickItem;
    if (!mlistFindTranslates.isEmpty())
    {
        lastStickItem = mlistFindTranslates.at(0);
    }
    for (int i = 0; i < mlistFindTranslates.size(); ++i)
    {
        if (item->msExtra == mlistFindTranslates.at(i)->msExtra
                && mlistFindTranslates.at(i)->mbStick)
        {
            lastStickItem = mlistFindTranslates.at(i);
            break;
        }
    }
    mlistFindTranslates.removeAll(item);
    mlistFindTranslates.prepend(item);
    return lastStickItem;
}

XYTranslateItem *XYTranslateModel::getItem(int index)
{
    if (mlistFindTranslates.size() > index)
    {
        return mlistFindTranslates.at(index);
    }
    else
    {
        return NULL;
    }
}

void XYTranslateModel::clear(bool del)
{
    if (del)
    {
        qDeleteAll(mlistFindTranslates);
    }
    mlistFindTranslates.clear();
}

bool XYTranslateModel::haveData(int index) const
{
    return index < mlistFindTranslates.size();
}

int XYTranslateModel::counts()
{
    return mlistFindTranslates.size();
}

QString XYTranslateModel::data(int index, int role) const
{
    if (index >= mlistFindTranslates.size())
    {
        return QString();
    }
    switch (role)
    {
    case SOURCE:
        return mlistFindTranslates.at(index)->msSource;
    case TRANSLATE:
        return mlistFindTranslates.at(index)->msTranslate;
    case COMPLETE:
        return mlistFindTranslates.at(index)->msComplete;
    case COMP_TRAN:
        return mlistFindTranslates.at(index)->msComplete + "  " + mlistFindTranslates.at(index)->msTranslate;
    case TRAN_COMP:
        return mlistFindTranslates.at(index)->msTranslate + "  " + mlistFindTranslates.at(index)->msComplete;
    case EXTRA:
        return mlistFindTranslates.at(index)->msExtra;
    case TIMES:
        return QString::number(mlistFindTranslates.at(index)->miTimes);
    default:
        return QString();
    }
}

