
#include "imageprocessorwidget.h"
#include <QPainter>
#include <QLabel>
#include <QToolButton>
#include "systempath.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QRect>
#include "filemanagerview.h"
#include <QDebug>
#include <QModelIndex>
#include <QApplication>
#include "showpicturewidget.h"


ImageProcessorWidget::ImageProcessorWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    setupUI();
}

ImageProcessorWidget::~ImageProcessorWidget()
{
}

void ImageProcessorWidget::setupUI()
{
    showPictureWidget = new eink::ShowPictureWidget(this);

    FileManagerView *fileview = new FileManagerView(this);
    fileview->setFileType(FILE_TYPE::FILE_IMAGE);
    connect(fileview,SIGNAL(doubleClicked(const QModelIndex &)),
        this,SLOT(doubleClickedsLOT(const QModelIndex &)));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,1,0,0);
    hLayout->addWidget(showPictureWidget,3);
    hLayout->setSpacing(0);
    hLayout->addWidget(fileview,1);

    QVBoxLayout *mainVlayout = new QVBoxLayout(this);
    mainVlayout->setContentsMargins(1,1,1,1);
    //mainVlayout->addWidget(titleBar,1);
    mainVlayout->setSpacing(0);
    mainVlayout->addLayout(hLayout);
    setLayout(mainVlayout);
}

void ImageProcessorWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawLine(0, 0, this->width() - 1, 0);
    painter.drawLine(0, 0, 0, this->height() - 1);
    painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
    painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);
}

void ImageProcessorWidget::resetEvn()
{
}


void ImageProcessorWidget::doubleClickedsLOT(const QModelIndex &index)
{
    QString filePath = index.data(1).toString();
    qDebug() << "ProcessorWidget::doubleClickedsLOT selected file path===" << filePath;

    showPictureWidget->setFilePath(filePath);
}


void ImageProcessorWidget::ExitApplication()
{
    resetEvn();
}

