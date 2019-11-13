
#include "filemanagerview.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>
#include "listItem.h"
#include <QStandardItem>
#include <QHBoxLayout>
#include "listview.h"
#include "systempath.h"

FileManagerView::FileManagerView(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

FileManagerView::~FileManagerView()
{
    fileStrList.clear();
}

void FileManagerView::setupUI()
{
    QPushButton *addFileBut = new QPushButton(this);
    addFileBut->setText(QString::fromLocal8Bit("添加文件"));
    connect(addFileBut,SIGNAL(clicked()),this,SLOT(addFileSlot()));
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(addFileBut);

    fileListView = new eink::ListView(this);
    fileListView->setContentsMargins(10,0,0,0);
    fileListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fileListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(fileListView,SIGNAL(doubleClicked(const QModelIndex &)),
                this,SLOT(doubleClickedSlot(const QModelIndex &)));

    model = new QStandardItemModel(this);
    fileListView->setModel(model);
    fileListView->setItemDelegate(new ListItemDelegate(this));
    fileListView->setMouseTracking(false);

    QVBoxLayout *mainVlayout = new QVBoxLayout(this);
    mainVlayout->setContentsMargins(0,0,0,0);
    mainVlayout->addWidget(fileListView,10);
    mainVlayout->addLayout(hLayout,1);
    setLayout(mainVlayout);
}

void FileManagerView::addFileSlot()
{
    //创建一个QFileDialog对象，构造函数中的参数可以有所添加
    QFileDialog fileDialog ;

    fileDialog.setWindowTitle(tr("Save As"));//设置文件保存对话框的标题
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);//设置文件对话框为保存模式
    fileDialog.setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
    fileDialog.setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信息；

    //还有另一种形式QFileDialog::List，这个只是把文件的文件名以列表的形式显示出来
    //fileDialog->setGeometry(10,30,300,200);//设置文件对话框的显示位置
    QString startDir = SystemPath::getFilesPath();
    fileDialog.setDirectory(startDir);//设置文件对话框打开时初始打开的位置

    //设置文件类型过滤器
    QString fileTypeList;

    switch(fileType) {
        case FILE_TYPE::FILE_AUDIO:
        {
            fileTypeList = "*.mp3 *.flac *.wav *.mp4";
            break;
        }
        case FILE_TYPE::FILE_VEDIO:
        {
            fileTypeList = "*.mp4 *.avi";
            break;
        }
        case FILE_TYPE::FILE_IMAGE:
        {
            fileTypeList = "*.png *.jpg";
            break;
        }
        default:
            break;
    }

    //过滤文件
    //fileDialog.setNameFilters(fileTypeList);
    fileDialog.setNameFilter(fileTypeList);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    QStringList fileList;

    if(fileDialog.exec() == QDialog::Accepted)
    {
        //注意使用的是QFileDialog::Accepted或者QDialog::Accepted,不是QFileDialog::Accept
        fileList = fileDialog.selectedFiles();
    }

    for(int i = 0;i < fileList.size();i++) {
        QString filePath = fileList[i];
        if(fileStrList.indexOf(filePath) < 0){
            fileStrList.append(filePath);
            addItemData(filePath);
        }
    }
}

void FileManagerView::addItemData(const QString &data)
{
    QStandardItem *item = new QStandardItem;
    item->setData(data,1);
    model->appendRow(item);
}

void FileManagerView::setFileType(FILE_TYPE type)
{
    fileType = type;
}

void FileManagerView::doubleClickedSlot(const QModelIndex &index)
{
    fileListView->setCurrentIndex(index);
    int curModelIndexRow = index.row();
    int curModelIndexCol = index.column();
    QStandardItem *item = model->item(curModelIndexRow,curModelIndexCol);
    item->setBackground(QBrush(Qt::red));
    model->setItem(curModelIndexRow,curModelIndexCol,item);
    emit doubleClicked(index);
}

void FileManagerView::sendNextFileMes()
{
    int modelRowNum = model->rowCount();
    int modelColNum = model->columnCount();

    QModelIndex curModelIndex = fileListView->currentIndex();
    int curModelIndexRow = curModelIndex.row();
    int curModelIndexCol = curModelIndex.column();
    QStandardItem *item = model->item(curModelIndexRow,curModelIndexCol);
    item->setBackground(QBrush(Qt::white));
    model->setItem(curModelIndexRow,curModelIndexCol,item);

    qDebug() << "FileManagerView::sendNextFileMes() model totle row,col=" << model->rowCount()
             << model->columnCount() << "cur index row,col=" << curModelIndexRow << curModelIndexCol;

    curModelIndexRow += 1;

    if(curModelIndexRow >= modelRowNum)
        curModelIndexRow = 0;

    qDebug() << "FileManagerView::sendNextFileMes() model totle row,col=" << model->rowCount()
             << model->columnCount() << "cur index row,col=" << curModelIndexRow << curModelIndexCol;

    item = model->item(curModelIndexRow,curModelIndexCol);
    item->setBackground(QBrush(Qt::red));
    model->setItem(curModelIndexRow,curModelIndexCol,item);

    curModelIndex =  model->index(curModelIndexRow,curModelIndexCol);
    fileListView->setCurrentIndex(curModelIndex);
    emit doubleClicked(curModelIndex);
}

void FileManagerView::sendLastFileMes()
{
    int modelRowNum = model->rowCount();
    int modelColNum = model->columnCount();

    QModelIndex curModelIndex = fileListView->currentIndex();
    int curModelIndexRow = curModelIndex.row();
    int curModelIndexCol = curModelIndex.column();
    QStandardItem *item = model->item(curModelIndexRow,curModelIndexCol);
    item->setBackground(QBrush(Qt::white));
    model->setItem(curModelIndexRow,curModelIndexCol,item);

    curModelIndexRow -= 1;
    if(curModelIndexRow < 0)
        return;

    item = model->item(curModelIndexRow,curModelIndexCol);
    item->setBackground(QBrush(Qt::red));
    model->setItem(curModelIndexRow,curModelIndexCol,item);
    curModelIndex =  model->index(curModelIndexRow,curModelIndexCol);
    fileListView->setCurrentIndex(curModelIndex);
    emit doubleClicked(curModelIndex);
}

QString FileManagerView::getNextFilePath()
{
    QString str;
    return str;
}

QString FileManagerView::getLastFilePath()
{
    QString str;
    return str;
}
