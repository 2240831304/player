
#include "mainwidget.h"
#include <QPainter>
#include <QLabel>
#include <QToolButton>
#include "systempath.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "playwidget.h"
#include <QMouseEvent>
#include <QRect>
#include "filemanagerview.h"
#include <QDebug>
#include <QModelIndex>
#include "playmanager.h"
#include <QApplication>
#include <QTimer>
#include <QTime>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(800,600);

    playerMgr = NULL;
    playerWidget = NULL;

    setupUI();

}

MainWidget::~MainWidget()
{
    if(playerMgr)
        delete playerMgr;
    playerMgr = NULL;
}

void MainWidget::setupUI()
{
    TitleBar *titleBar = new TitleBar(this);
    connect(titleBar,SIGNAL(close()),this,SLOT(closeSlot()));
    connect(titleBar,SIGNAL(minimize()),this,SLOT(minimizeSlot()));

    playerWidget = new eink::PlayWidget(this);

    FileManagerView *fileview = new FileManagerView(this);
    connect(fileview,SIGNAL(doubleClicked(const QModelIndex &)),
        this,SLOT(doubleClickedsLOT(const QModelIndex &)));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,1,0,0);
    hLayout->addWidget(playerWidget,3);
    hLayout->setSpacing(0);
    hLayout->addWidget(fileview,1);

    QVBoxLayout *mainVlayout = new QVBoxLayout(this);
    mainVlayout->setContentsMargins(1,1,1,1);
    mainVlayout->addWidget(titleBar,1);
    mainVlayout->setSpacing(0);
    mainVlayout->addLayout(hLayout,9);
    setLayout(mainVlayout);
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawLine(0, 0, this->width() - 1, 0);
    painter.drawLine(0, 0, 0, this->height() - 1);
    painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
    painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);
}

void MainWidget::closeSlot()
{
    resetEvn();
    quitApp();
}

void MainWidget::quitApp()
{
    qApp->quit();
}

void MainWidget::resetEvn()
{
    if(playerWidget)
        playerWidget->restartPlay();

    if(playerMgr)
        playerMgr->stopPlay();

}


void MainWidget::doubleClickedsLOT(const QModelIndex &index)
{
    static QTime startTime = QTime::currentTime();

    QString filePath = index.data(1).toString();
    qDebug() << "MainWidget::doubleClickedsLOT selected file path===" << filePath;

    if(playerMgr) {
        QTime curTime = QTime::currentTime();;
        if(startTime.msecsTo(curTime) < 1000) {
            startTime = curTime;
            return;
        }
    }

    resetEvn();

    if(!playerMgr)
        playerMgr = new eink::PlayManager;
    playerMgr->setPlayFilePath(filePath);
    playerMgr->startPlay();

    playerWidget->startPlay();
}

void MainWidget::minimizeSlot()
{
    showMinimized();
}




//顶部灰色的区域，内容
TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

TitleBar::~TitleBar()
{
}

void TitleBar::setupUI()
{
    QLabel *title = new QLabel(this);
    title->setText(QString::fromLocal8Bit("视屏播放器"));

    QToolButton *closeBut = new QToolButton(this);
    QString closePic = SystemPath::getResourcePath() + "/pic/close.png";
    closeBut->setFixedSize(30,30);
    closeBut->setIcon(QIcon(closePic));
    connect(closeBut,SIGNAL(clicked()),SIGNAL(close()));

    QToolButton *minimizeBut = new QToolButton(this);
    QString minimizePic = SystemPath::getResourcePath() + "/pic/minimize.jpg";
    minimizeBut->setFixedSize(20,20);
    minimizeBut->setIcon(QIcon(minimizePic));
    connect(minimizeBut,SIGNAL(clicked()),SIGNAL(minimize()));

    QHBoxLayout *mainHlayout = new QHBoxLayout(this);
    mainHlayout->setContentsMargins(10,1,10,1);
    mainHlayout->addWidget(title);
    mainHlayout->addWidget(minimizeBut);
    mainHlayout->addWidget(closeBut);
    setLayout(mainHlayout);
}

void TitleBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(),QColor(Qt::gray));
}

void TitleBar::mousePressEvent (QMouseEvent *event)
{

   if (event->button() == Qt::LeftButton)
   {
        mousePress = true;
        QPoint tpPos = event->pos();
        pressedPos = event->globalPos();
        formerMainPos.setX(pressedPos.x() - tpPos.x());
        formerMainPos.setY(pressedPos.y() - tpPos.y());
//       QRect rect(0,0,width(),40);
//       if(rect.contains(event->pos()))
//       {
//           mousePress = true;
//       }
   }
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePress = false;
    }
}

void TitleBar::mouseMoveEvent (QMouseEvent * event)
{

    if (mousePress)
    {
        QPoint currentPos = event->globalPos();
        int newx = currentPos.x()-pressedPos.x() + formerMainPos.x();
        int newy = currentPos.y()-pressedPos.y() + formerMainPos.y();
        this->parentWidget()->move(newx,newy);
        pressedPos = currentPos;
        formerMainPos.setX(newx);
        formerMainPos.setY(newy);
    }
}