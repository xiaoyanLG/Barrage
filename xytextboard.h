#ifndef XYTEXTBOARD_H
#define XYTEXTBOARD_H

#include "xymovablewidget.h"

class QTextEdit;
class XYTextBoard : public XYMovableWidget
{
    Q_OBJECT
public:
    explicit XYTextBoard(QWidget *parent = 0);
    ~XYTextBoard();

protected:
    void closeEvent(QCloseEvent *event);

private:
    QTextEdit *mopTextEdit;

};

#endif // XYTEXTBOARD_H
