#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbarragescreen.h"
#include <QTime>
#include "systemtray.h"
#include "cdrawwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <QTextBlock>
#include <QTextFormat>
#include <QFileInfo>
#include <QTimer>
#include <QTextDocumentWriter>
#include "cemoticonwidget.h"
#include "csignalbarragescreen.h"

#include <QDebug>

MainWindow *MainWindow::mopInstance = NULL;
MainWindow *MainWindow::instance()
{
    return mopInstance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    SystemTray *tray = new SystemTray(this);
    tray->show();
    ui->setupUi(this);
    connect(ui->pushButton_send, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(ui->pushButton_draw, SIGNAL(clicked()), this, SLOT(getPoints()));
    connect(ui->pushButton_backImage, SIGNAL(clicked()), this, SLOT(setBackImage()));
    connect(ui->pushButton_insertImage, SIGNAL(clicked()), this, SLOT(insertImage()));
    connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(addAnimation()));

    miTimer = startTimer(1000*60);
        ui->textEdit->clear();
        insertImage(":/Gif/60");
    resize(1000, 700);
    mopInstance = this;
}

QFont MainWindow::getFont()
{
    return ui->fontComboBox->currentFont();
}

MainWindow::~MainWindow()
{
    delete ui;
}

CBarrageItem *MainWindow::getItem(int time)
{
    qsrand(QTime::currentTime().msecsTo(QTime(0, 0)) + time * 99);
    CBarrageItem *item = new CBarrageItem;
    item->setShowTimes(ui->lineEdit_time->text().toInt()
                       , ui->lineEdit_counts->text().toInt());

    QTextFrame *frame = ui->textEdit->document()->rootFrame();
    auto it = frame->begin();
    Contents *contentsHeader = NULL;
    Contents *contentsNext = NULL;
    for (; !it.atEnd(); ++it)
    {
        QTextBlock block = it.currentBlock();
        if (block.isValid())
        {
            QTextBlock::Iterator it1 = block.begin();
            for (; !it1.atEnd(); ++it1)
            {
                const QTextCharFormat format = it1.fragment().charFormat();
                QString txt = it1.fragment().text();
                const bool isObject = txt.contains(QChar::ObjectReplacementCharacter);
                const bool isImage = isObject && format.isImageFormat();
                if (isImage && isObject)
                {
                    for (int i = 0; i < txt.length(); ++i)
                    {
                        if (format.toImageFormat().isValid())
                        {
                            Contents *next = new Contents;
                            next->type = Contents::PIXMAP;
                            next->pixmap.setFileName(QUrl(format.toImageFormat().name()).toLocalFile());
                            if (next->pixmap.fileName().startsWith("/:/"))
                            {
                                next->pixmap.setFileName(next->pixmap.fileName().remove(0, 1));
                            }
                            if (contentsHeader == NULL)
                            {
                                contentsHeader = next;
                            }
                            else if (contentsNext == NULL)
                            {
                                contentsNext = next;
                                contentsHeader->next = contentsNext;
                            }
                            else
                            {
                                contentsNext->next = next;
                                contentsNext = next;
                            }
                        }
                    }
                }
                else
                {
                    Contents *next = new Contents;
                    next->type = Contents::TEXT;
                    next->text = txt;
                    if (contentsHeader == NULL)
                    {
                        contentsHeader = next;
                    }
                    else if (contentsNext == NULL)
                    {
                        contentsNext = next;
                        contentsHeader->next = contentsNext;
                    }
                    else
                    {
                        contentsNext->next = next;
                        contentsNext = next;
                    }
                }
            }
        }

        // 添加换行符
        if (contentsHeader != NULL)
        {
            Contents *next = new Contents;
            next->type = Contents::LF;
            next->text = "\n";
            if (contentsNext == NULL)
            {
                contentsNext = next;
                contentsHeader->next = contentsNext;
            }
            else
            {
                contentsNext->next = next;
                contentsNext = next;
            }
        }
    }

    item->setContents(contentsHeader);
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox_size->value());
    item->setTextFont(font);
    QColor color(ui->spinBox_R->value(),
                 ui->spinBox_G->value(),
                 ui->spinBox_B->value());
    item->setTextColor(color);

    int y = ui->lineEdit_Y->text().toInt();
    if (y == -1)
    {
        y = qrand() %(QApplication::desktop()->height() - 100);
    }
    item->setStartPos(QPoint(ui->lineEdit_X->text().toInt()
                             , y));

    item->setEndPos(QPoint(ui->lineEdit_EX->text().toInt()
                             , ui->lineEdit_EY->text().toInt()));

    item->setBarrageSize(ui->lineEdit_limitWidth->text().toInt()
                         , ui->lineEdit_limitHeight->text().toInt());

    if (ui->checkBox_rand->isChecked())
    {
        item->setAnimation((QEasingCurve::Type)(qrand() % 41));
    }
    else
    {
        item->setAnimation((QEasingCurve::Type)qMax(ui->listWidget->currentIndex().row(), 0));
    }

    item->setBackImage(moBackImage);

    item->setDrawPoints(mlistDrawPoints);

    return item;
}

void MainWindow::addItem()
{
    addItem(0);
}

void MainWindow::addItem(int time)
{
    CBarrageItem *item = getItem(time);
    CBarrageScreen::getScreen()->setMaxBarrageNumber(ui->lineEdit_max->text().toInt());
    CBarrageScreen::getScreen()->addItem(item);
}

void MainWindow::getPoints()
{
    DrawWidget draw;
    draw.showFullScreen();
    draw.exec();
    mlistDrawPoints = draw.getDrawPoints();
    mfTotalLength  = draw.getTotalLength();
    mlistPointSpaces = draw.getDrawPointSpaces();
}

void MainWindow::setBackImage()
{
    QString imageFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     QString(),
                                                     tr("Images (*.png *.xpm *.jpg *.ico);;All (*.*)"));
    moBackImage.setFileName(imageFile);
}

void MainWindow::insertImage(const QString &filename)
{
    QPixmap pixmap(filename);

    QUrl Uri ( QString ( "file:///%1" ).arg ( filename ) );
    QTextDocument * textDocument = ui->textEdit->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant (pixmap) );
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( pixmap.width());
    imageFormat.setHeight( pixmap.height());
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);
}

void MainWindow::insertImage()
{
    static CEmoticonWidget *widget = new CEmoticonWidget;
    static bool first = true;
    if (first)
    {
        first = false;
        connect(widget, SIGNAL(clicked(QString)), this, SLOT(insertImage(QString)));
    }
    widget->show();
}

void MainWindow::timerShot(int number)
{
    for (int i = 0; i < number; ++i)
    {
        addItem(i);
    }
}

void MainWindow::addAnimation()
{
    CSignalBarrageScreen *signalScreen = new CSignalBarrageScreen;
    signalScreen->setItem(getItem());
    signalScreen->show();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (miTimer == event->timerId())
    {
        int time = ui->spinBox_time->value();
        if (time == 1)
        {
            int number = ui->spinBox_number->value();
            bool loop = ui->checkBox_loop->isChecked();
            int times = ui->lineEdit_time->text().toInt();
            timerShot(number);
            if (loop)
            {
                miLoopShot = startTimer(times);
            }
            else
            {
                killTimer(miLoopShot);
            }
            ui->spinBox_time->setValue(--time);
        }
        else if (time > 0)
        {
            ui->spinBox_time->setValue(--time);
        }
    }
    else if (miLoopShot == event->timerId())
    {
        int number = ui->spinBox_number->value();
        bool loop = ui->checkBox_loop->isChecked();
        if (!loop)
        {
            killTimer(miLoopShot);
        }
        timerShot(number);
    }
}
