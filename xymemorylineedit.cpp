#include "xymemorylineedit.h"
#include <QCompleter>

XYMemoryLineEdit::XYMemoryLineEdit(QWidget *parent)
{

}

void XYMemoryLineEdit::saveCurrentText()
{
    if (mlistMemoryString.size() < 10 && !mlistMemoryString.contains(text()))
    {
        mlistMemoryString.prepend(text());
    }
}

void XYMemoryLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (selectedText().isEmpty())
    {
//        qSort(mlistMemoryString.begin(), mlistMemoryString.end());
        setCompleter(new QCompleter(mlistMemoryString));
        completer()->complete();
        completer()->popup();
    }
    QLineEdit::mouseReleaseEvent(event);
}
