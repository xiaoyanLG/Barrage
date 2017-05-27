#include "xytextboard.h"
#include <QTextEdit>
#include <QVBoxLayout>

XYTextBoard::XYTextBoard(QWidget *parent)
    : XYMovableWidget(parent)
{
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    mopTextEdit = new QTextEdit;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mopTextEdit);

    setLayout(layout);
    setWindowTitle(tr("TextBoard"));
}

XYTextBoard::~XYTextBoard()
{

}

void XYTextBoard::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    deleteLater();
}
