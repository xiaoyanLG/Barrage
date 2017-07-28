#ifndef XYINPUT_H
#define XYINPUT_H

#include "xybordershadowwidget.h"
#include "xytranslateview.h"
#include <QMap>

class QLineEdit;
class XYInput : public XYBorderShadowWidget
{
    Q_OBJECT
public:
    static XYInput *getInstance();
    explicit XYInput(QWidget *parent = 0);
    bool initInputBase(const QString &path);
    ~XYInput();

signals:
    void complete(const QString &out);

public slots:
    void setEnglish(bool english);
    void showMoreInfo();
    bool close();
    void show();

private slots:
    void mslotFindTranslate(const QString &keyword); // 查找输入内容对应的词（中英文都通过这个接口）

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    // 分割拼音，比如women-->wo%'men,返回分割后的字符串(并加上%模糊查找)，并带回有效的字数
    QString splitePinyin(const QString &pinyin, int &num);
    void load();           // 装载查找到的内容，并显示出来
    void deDuplication(QList<XYTranslateItem *> &items, bool del = true); // 删除重复的字词
    XYTranslateItem *autoCreateWords(const QString &keyword);  // 自动造词
    QList<XYTranslateItem *> findItemsFromTemp(const QString &keyword, bool force = true);
    QList<XYTranslateItem *> findPossibleMust(const QString &keyword, int max = 200);
    void completeInput(const QString &text, XYTranslateItem *item = NULL);
    void clearTemp();
    void saveItem(XYTranslateItem *item);
    QStringList getYunMuByShengMu(const QChar &shenmu);

private:
    static XYInput  *mopInstance;
    QWidget         *mopLatestWidget;
    QLineEdit       *mopLineEdit;
    bool             mbEnglish;

    XYTranslateItem  moCompleteItem;
    XYTranslateItem  moAutoCompleteItem;
    QString          msCurrentKeyWords;
    QMap<QString, QList<XYTranslateItem *> > mmapTempItems; // 存储一次输入中查询过的词组
    XYTranslateView *mopTransLateView;

    friend class XYTranslateView;
};

#endif // XYINPUT_H
