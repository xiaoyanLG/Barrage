#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xybarragescreen.h"
#include <QTime>
#include "xysystemtray.h"
#include "xydrawwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <QTextBlock>
#include <QTextFormat>
#include <QFileInfo>
#include <QTimer>
#include <QPlainTextEdit>
#include <QTextDocumentWriter>
#include "xyemoticonwidget.h"
#include "xysignalbarragescreen.h"
#include "xytooltips.h"

// 斗鱼弹幕
#include "danmuconfig.h"
#include "jsonparse.h"
#include "stringgenerator.h"

MainWindow *MainWindow::mopInstance = NULL;
MainWindow *MainWindow::instance()
{
    return mopInstance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    XYSystemTray *tray = new XYSystemTray(this);
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

    // 斗鱼弹幕
    network_access = new NetworkAccess();
    tcpSocket = new DouyuTcpSocket(this);
    connect(ui->monitorBtn, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->stopBtn, SIGNAL(clicked()), this, SLOT(stop()));
    connect(network_access, SIGNAL(pageLoadFinished(QString)),
            this, SLOT(htmlContent(QString)));
    connect(tcpSocket, SIGNAL(chatMessage(QMap<QString,QString>)), this, SLOT(showChatMessage(QMap<QString,QString>)));
}

QFont MainWindow::getFont()
{
    return ui->fontComboBox->currentFont();
}

MainWindow::~MainWindow()
{
    delete ui;
}

XYBarrageItem *MainWindow::getItem(int time)
{
    qsrand(QTime::currentTime().msecsTo(QTime(0, 0)) + time * 99);
    XYBarrageItem *item = new XYBarrageItem;
    int showTimes = ui->lineEdit_time->text().toInt();
    if (ui->checkBox_time->isChecked())
    {
        showTimes = (qrand() % 20 + 5) * 1000;
    }
    item->setShowTimes(showTimes
                       , ui->lineEdit_counts->text().toInt());

    QTextFrame *frame = ui->textEdit->document()->rootFrame();
    auto it = frame->begin();
    XYContents *contentsHeader = NULL;
    XYContents *contentsNext = NULL;
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
                            XYContents *next = new XYContents;
                            next->type = XYContents::PIXMAP;
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
                    XYContents *next = new XYContents;
                    next->type = XYContents::TEXT;
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
            XYContents *next = new XYContents;
            next->type = XYContents::LF;
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
    if (ui->checkBox_font->isChecked())
    {
        font = QFont(ui->fontComboBox->itemText(qrand() % ui->fontComboBox->count()));
    }
    font.setPointSize(ui->spinBox_size->value());
    if (ui->checkBox_size->isChecked())
    {
        font.setPointSize(qrand() % 40 + 10);
    }
    item->setTextFont(font);
    QColor color;
    if (ui->checkBox_color->isChecked())
    {
        color = QColor(qrand() % 256,
                       qrand() % 256,
                       qrand() % 256);
    }
    else
    {
        color = QColor(ui->spinBox_R->value(),
                       ui->spinBox_G->value(),
                       ui->spinBox_B->value());
    }
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
    XYBarrageItem *item = getItem(time);
    XYBarrageScreen::getScreen()->setMaxBarrageNumber(ui->lineEdit_max->text().toInt());
    XYBarrageScreen::getScreen()->addItem(item);
}

void MainWindow::getPoints()
{
    XYDrawWidget draw;
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
    static XYEmoticonWidget *widget = new XYEmoticonWidget;
    static bool first = true;
    if (first)
    {
        first = false;
        connect(widget, SIGNAL(clicked(QString)), this, SLOT(insertImage(QString)));
    }
    widget->show();
    widget->raise();
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
    XYSignalBarrageScreen *signalScreen = new XYSignalBarrageScreen;
    signalScreen->setItem(getItem());
    signalScreen->show();
}

void MainWindow::htmlContent(const QString html)
{
    //正则数据提取JSON
    QString pattern = _Douyu_Room_Pattern;
    QRegExp regExp(pattern);
    regExp.setMinimal(true);
    QString json;
    int pos = 0;
    while((pos = regExp.indexIn(html,pos)) != -1)
    {
        json = regExp.capturedTexts().at(1);
        pos += regExp.matchedLength();
    }
    JSONParse parse;
    if(parse.init(json))
    {
        QString roomid = parse.getJsonValue(_Douyu_RoomId);
        tcpSocket->connectDanmuServer(roomid);
    }
}


void MainWindow::showChatMessage(QMap<QString,QString> messageMap)
{
//    qDebug() << __FUNCTION__;
    QString str = StringGenerator::getString(messageMap,
                                             true,
                                             ui->checkBox_name->isChecked(),
                                             ui->checkBox_grade->isChecked());

    if ( messageMap["type"] == "connectstate")
    {//连接状态
        ui->connectState->setText(messageMap["state"]);
        return;
    }
    else if( messageMap["type"] == "chatmsg")
    {//弹幕消息

    }
    else if( messageMap["type"] == "dgb" || messageMap["type"] == "bc_buy_deserve")
    {//打赏消息
        if (!ui->checkBox_gift->isChecked())
        {
            return;
        }
    }

    XYBarrageItem *item = getItem(0);
    XYContents *contentsHeader =new XYContents(str.trimmed());
    XYContents *next = new XYContents;
    next->type = XYContents::LF;
    next->text = "\n";
    contentsHeader->next = next;
    item->setContents(contentsHeader);
    XYBarrageScreen::getScreen()->setMaxBarrageNumber(ui->lineEdit_max->text().toInt());
    XYBarrageScreen::getScreen()->addItem(item);
}

void MainWindow::start()
{
    QString roomid = ui->roomID->text();
    QRegExp rx("[0-9a-zA-Z]+");
    rx.setMinimal(false);
    if(rx.exactMatch(roomid))
    {
        bool ok = false;
        roomid.toInt(&ok);
        if(!ok)
        {
            QString url_str = QString("http://www.douyu.com/%1").arg(roomid);
            QUrl url = QUrl(QString(url_str));
            network_access->loadingPage(url);
        }
        else
        {
            tcpSocket->connectDanmuServer(roomid);
        }
    }
}

void MainWindow::stop()
{
    tcpSocket->stop();
    ui->roomID->clear();
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
