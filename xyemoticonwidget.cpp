#include "xyemoticonwidget.h"
#include "xyexpressionlabel.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMovie>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileDialog>

XYEmoticonWidget::XYEmoticonWidget(QWidget *parent)
    : QDialog(parent)
{ 
    QGridLayout *layout = new QGridLayout;
    for (int i = 0; i < 88; ++i)
    {
        XYExpressionLabel *label = new XYExpressionLabel;
        connect(label, SIGNAL(clicked(QString)), this, SIGNAL(clicked(QString)));
        QMovie *move = new QMovie;
        move->setFileName(QString(":/Gif/%1").arg(i));
        move->start();
        label->setMovie(move);
        layout->addWidget(label, i % 10, i / 10, Qt::AlignCenter);
    }
    for (int i = 0; i < 66; ++i)
    {
        XYExpressionLabel *label = new XYExpressionLabel;
        connect(label, SIGNAL(clicked(QString)), this, SIGNAL(clicked(QString)));
        QMovie *move = new QMovie;
        move->setFileName(QString(":/Gif1/%1").arg(i));
        move->start();
        label->setMovie(move);
        layout->addWidget(label, (i + 88) % 10, (i + 88) / 10, Qt::AlignCenter);
    }
    for (int i = 0; i < 104; ++i)
    {
        XYExpressionLabel *label = new XYExpressionLabel;
        connect(label, SIGNAL(clicked(QString)), this, SIGNAL(clicked(QString)));
        QMovie *move = new QMovie;
        move->setFileName(QString(":/Gif2/%1").arg(i));
        move->start();
        label->setMovie(move);
        layout->addWidget(label, (i + 154) % 10, (i +154) / 10, Qt::AlignCenter);
    }
    QWidget *emo = new QWidget;
    emo->setLayout(layout);
    QScrollArea *area = new QScrollArea;
    area->setWidget(emo);

    QDialogButtonBox *box = new QDialogButtonBox;
    QPushButton *open = new QPushButton("Open other");
    connect(open, SIGNAL(clicked()), this, SLOT(openImage()));
    QPushButton *ok = new QPushButton("Close");
    connect(ok, SIGNAL(clicked()), this, SLOT(close()));
    box->addButton(open, QDialogButtonBox::ResetRole);
    box->addButton(ok, QDialogButtonBox::YesRole);
    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addWidget(area);
    layout_main->addWidget(box, Qt::AlignRight);
    setLayout(layout_main);
    resize(900, 600);
}

void XYEmoticonWidget::openImage()
{
    QString imageFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     QString(),
                                                     tr("Images (*.png *.xpm *.jpg *.ico);;All (*.*)"));
    emit clicked(imageFile);
}
