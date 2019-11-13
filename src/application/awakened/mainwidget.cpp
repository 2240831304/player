
#include "mainwidget.h"
#include <QPainter>
#include <QLabel>
#include <QToolButton>
#include "systempath.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QRect>
#include <QDebug>
#include <QModelIndex>
#include <QApplication>
#include <QTimer>
#include <QTime>
#include "appdisplayshelf.h"
#include "appwidget.h"


class MainWidgetPrivate
{
    public:
        eink::AppDisplayShelf *appUIWidget;
        eink::AppWidget *appWidget;
};


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),d(new MainWidgetPrivate)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(800,600);

    setupUI();
}

MainWidget::~MainWidget()
{
}

void MainWidget::setupUI()
{
    TitleBar *titleBar = new TitleBar(this);
    connect(titleBar,SIGNAL(close()),this,SLOT(closeSlot()));
    connect(titleBar,SIGNAL(minimize()),this,SLOT(minimizeSlot()));

    d->appUIWidget = new eink::AppDisplayShelf(this);
    d->appWidget = new eink::AppWidget(this);
    connect(d->appWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,
                SLOT(itemDoubleClickedSlot(QListWidgetItem *)));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addWidget(d->appUIWidget,5);
    hLayout->setSpacing(0);
    hLayout->addWidget(d->appWidget,1);


    QVBoxLayout *mainVlayout = new QVBoxLayout(this);
    mainVlayout->setContentsMargins(0,0,0,0);
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
    d->appUIWidget->closeApps();
    quitApp();
}

void MainWidget::quitApp()
{
    qApp->quit();
}

void MainWidget::minimizeSlot()
{
    showMinimized();
}

void MainWidget::itemDoubleClickedSlot(QListWidgetItem *item)
{
    //应用程序的英文，中文名字
    QString appUSname = item->data(3).toString();
    QString appZHname = item->data(4).toString();
    d->appUIWidget->addApplication(appUSname,appZHname);
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
    title->setText(QString::fromLocal8Bit("探索者世界"));

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
