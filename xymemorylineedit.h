#ifndef XYMEMORYLINEEDIT_H
#define XYMEMORYLINEEDIT_H

#include <QLineEdit>

class XYMemoryLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    XYMemoryLineEdit(QWidget *parent = NULL);

public slots:
    void saveCurrentText();

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QStringList mlistMemoryString;
};

#endif // XYMEMORYLINEEDIT_H
