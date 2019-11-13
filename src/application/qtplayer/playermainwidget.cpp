
#include "playermainwidget.h"
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
#include <QApplication>

PlayerMainWidget::PlayerMainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    //setFixedSize(800,600);

    playerWidget = NULL;

    setupUI();
}

PlayerMainWidget::~PlayerMainWidget()
{
}

void PlayerMainWidget::setupUI()
{
    playerWidget = new eink::PlayWidget(this);

    FileManagerView *fileview = new FileManagerView(this);
    fileview->setFileType(FILE_TYPE::FILE_VEDIO);
    connect(fileview,SIGNAL(doubleClicked(const QModelIndex &)),
        this,SLOT(doubleClickedsLOT(const QModelIndex &)));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,1,0,0);
    hLayout->addWidget(playerWidget,3);
    hLayout->setSpacing(0);
    hLayout->addWidget(fileview,1);

    QVBoxLayout *mainVlayout = new QVBoxLayout(this);
    mainVlayout->setContentsMargins(1,1,1,1);
    //mainVlayout->addWidget(titleBar,1);
    mainVlayout->setSpacing(0);
    mainVlayout->addLayout(hLayout);
    setLayout(mainVlayout);
}

void PlayerMainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawLine(0, 0, this->width() - 1, 0);
    painter.drawLine(0, 0, 0, this->height() - 1);
    painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
    painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);
}

void PlayerMainWidget::resetEvn()
{
    if(playerWidget)
        playerWidget->stopPlay();
}


void PlayerMainWidget::doubleClickedsLOT(const QModelIndex &index)
{
    QString filePath = index.data(1).toString();
    qDebug() << "PlayerMainWidget::doubleClickedsLOT selected file path===" << filePath;

    playerWidget->setFilePath(filePath);
    playerWidget->startPlay();
}


void PlayerMainWidget::ExitApplication()
{
    resetEvn();
}

