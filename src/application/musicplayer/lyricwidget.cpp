
#include "lyricwidget.h"
#include "systempath.h"
#include <QPainter>
#include <QMap>
#include <QTime>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QListView>
#include <QStandardItemModel>
#include "itemdelegate.h"
#include <QScrollArea>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QScrollBar>
#include <QAbstractItemView>

namespace eink {
    class LyricWidgetPrivate
    {
        public:
            ~LyricWidgetPrivate(){}

            QImage backImage;
            QString lyricPath;
            QMap<int,QString> LyricContextMap;
            QMap<int,int> timePosIndexMap; //播放时间点的索引
            QListView *lyricListView;
            QStandardItemModel *model;
            QList<QLabel *> labelList;
            QScrollArea *scrollAreaWidget;
            QPlainTextEdit *plainTextEdit;
            QTextBrowser *textBrowser;
            QTextCursor text_cursor;
            int eachRowHeight;
            int offSetValue;
    };

   LyricWidget::LyricWidget(QWidget *parent)
        : QWidget(parent),d(new LyricWidgetPrivate)
   {
        //setupUI();

        //createUI();

        setWindowOpacity(0);
        setAttribute(Qt::WA_TranslucentBackground, true);

        //注册QTextCharFormat类来设置字体
        qRegisterMetaType<QTextCharFormat>("QTextCharFormat");

        makeUI();
   }

    LyricWidget::~LyricWidget()
    {
        if(d)
            delete d;
        d = NULL;
    }

    void LyricWidget::InitEvn()
    {
        d->LyricContextMap.clear();
        d->timePosIndexMap.clear();
        d->eachRowHeight = 0;
        d->offSetValue = 0;
    }

    void LyricWidget::createUI()
    {
        d->plainTextEdit = new QPlainTextEdit(this);
        QVBoxLayout *vLayoutMain = new QVBoxLayout(this);
        vLayoutMain->addWidget(d->plainTextEdit);
        setLayout(vLayoutMain);
    }

    void LyricWidget::makeUI()
    {
        d->textBrowser = new QTextBrowser(this);
        d->textBrowser->setWindowOpacity(0);
        d->textBrowser->setAttribute(Qt::WA_TranslucentBackground, true);
        //设置透明
        d->textBrowser->setStyleSheet("background:rgb(255,0,0,0);border:0px;");
        //设置滚动条不可见
        d->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        d->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QVBoxLayout *vLayoutMain = new QVBoxLayout(this);
        vLayoutMain->addWidget(d->textBrowser);
        setLayout(vLayoutMain);
    }

    void LyricWidget::setupUI()
    {
        d->lyricListView = new QListView(this);
        d->lyricListView->setContentsMargins(0,0,0,0);
        d->lyricListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        d->lyricListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        d->lyricListView->setAutoFillBackground(true);

        //d->lyricListView->setFocusPolicy(Qt::NoFocus);
        d->lyricListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        d->model = new QStandardItemModel(this);
        d->lyricListView->setModel(d->model);
        d->lyricListView->setItemDelegate(new ItemDelegate(this));

        QVBoxLayout *vLayoutMain = new QVBoxLayout(this);
        vLayoutMain->addWidget(d->lyricListView);
        setLayout(vLayoutMain);
    }

    void LyricWidget::paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
		//painter.setBrush(Qt::white);
		//painter.setPen(Qt::white);
		//painter.drawRect(0, 0, this->width(), this->height()); //先画成白色

        //根据窗口的宽高来将图片画在窗口上
        if(!d->backImage.isNull()) {
            d->backImage = d->backImage.scaled(width(), height(),
					Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter.drawImage(QPoint(0,0),d->backImage);
        }

    }

   void LyricWidget::setBackPicture(const QImage &image)
   {
        d->backImage = image;
        if(d->backImage.isNull()) {
            QString picPath = SystemPath::getResourcePath() + "/pic/lyric.png";
            d->backImage.load(picPath);
        }

        update();
   }

   void LyricWidget::setFileName(const QString &baseName)
   {
        QString path = SystemPath::getFilesPath() + "/lyric/" + baseName + ".lrc";
        d->lyricPath = path;
   }


    void LyricWidget::startPlayLyric()
    {
        InitEvn();

        parseLyricFile();
        //showLyricContent();

        showLyricText();
    }

    void LyricWidget::parseLyricFile()
    {
        QFile file(d->lyricPath);
        if(!file.exists()) {
            qDebug() << "LyricWidget::parseLyricFile() fiel not exists!!!!";
            return;
        }

        if(!file.open(QFile::ReadOnly | QIODevice::Text)) {
            qDebug() << "LyricWidget::parseLyricFile() open file faile!!!";
            return;
        }
        QTextStream istream(&file);

        QTime startTime(0,0,0,0);

        while (!istream.atEnd())
        {
            QString lineContext = istream.readLine();
            if(!lineContext.isEmpty()) {
                QStringList listStr = lineContext.split(QRegExp("[\[\\]]"));
                listStr.pop_front();
                listStr[0].append("0");
                QTime time = QTime::fromString(listStr[0], "mm:ss.zzz");
                int timePos = startTime.msecsTo(time) / 1000;
                d->LyricContextMap[timePos] = listStr[1];
                //qDebug() << "=================" << listStr[0] << startTime.msecsTo(time) << listStr[1];
            }
        }

        file.close();
    }

    void LyricWidget::showLyricContent()
    {
        d->model->clear();
        QList<QString> listStr = d->LyricContextMap.values();
        QList<int> tiemPosList = d->LyricContextMap.keys();
        for(int i = 0;i< listStr.size();i++) {
            addItemData(listStr[i]);
            d->timePosIndexMap[tiemPosList[i]] = i;
            qDebug() << "Swddedede==========" << tiemPosList[i] << i;
        }
    }

    void LyricWidget::addItemData(const QString &data)
    {
        qDebug() << "LyricWidget::addItemData value==" << data;
        QStandardItem *item = new QStandardItem;
        item->setData(data,1);
        d->model->appendRow(item);
    }

    void LyricWidget::showLyricText()
    {
        d->textBrowser->clear();
        d->textBrowser->setAlignment(Qt::AlignHCenter);
        d->textBrowser->setTextColor(QColor(Qt::black));

        //QFontMetrics fm(font);
        //qDebug() << "LyricWidget::showLyricText(),,widget,Height===" << this->width() << this->height();

        QList<QString> listStr = d->LyricContextMap.values();
        QList<int> tiemPosList = d->LyricContextMap.keys();

        int rowMaxWords = 0;
        for(int i = 0;i< listStr.size();i++) {
            d->timePosIndexMap[tiemPosList[i]] = i;
            d->textBrowser->append(listStr[i]);
            if(listStr[i].size() > rowMaxWords)
                rowMaxWords = listStr[i].size();
        }

        int fontWidthSize = (this->width() * 0.75) / (rowMaxWords * 1.2);
        //qDebug() << "djwmodjwodjwodjow==========" << fontWidthSize << rowMaxWords << this->width();
        QFont font;
        //设置文字大小像素
        font.setPixelSize(fontWidthSize);
        //设置文字大小
        //font.setPointSize(25);
        //设置字间距
        //font.setWeight(fontWidthSize);
        font.setLetterSpacing(QFont::PercentageSpacing,120);
        d->textBrowser->setFont(font);

        //qDebug() << "988888888888888888888888888= spacing=" << font.letterSpacing();

        d->text_cursor = d->textBrowser->textCursor();
        int browerTextRow = d->textBrowser->document()->lineCount();
        QScrollBar *bar = d->textBrowser->verticalScrollBar();
        int totleHeight = this->height() + bar->maximum();
        d->eachRowHeight = totleHeight / browerTextRow;
        d->offSetValue = this->height() / 3;
        int barMinValue = 0 - d->offSetValue;
        int barMaxValue = bar->maximum() + d->offSetValue * 2;
        bar->setRange(barMinValue,barMaxValue);

        d->text_cursor.movePosition(QTextCursor::Start);
        d->textBrowser->setTextCursor(d->text_cursor);

        setShowTimePos(0);
        //connect(d->textBrowser,SIGNAL(cursorPositionChanged()),this,SLOT(autoScrollSlot()));
    }

    void LyricWidget::setShowTimePos(int posTime)
    {
        QTextCharFormat fmt;
        int key = posTime / 1000;

        if(d->timePosIndexMap.find(key) != d->timePosIndexMap.end()) {
            int index = d->timePosIndexMap.value(key);
            //qDebug() << "LyricWidget::setShowTimePos========" << posTime << index;

            d->text_cursor.select(QTextCursor::LineUnderCursor);
            //选中要着色的内容
            fmt.setForeground(QColor(Qt::black));
            d->text_cursor.mergeCharFormat(fmt); //设置文本格式
            d->text_cursor.clearSelection(); //撤销选中

            while( index != d->text_cursor.blockNumber()) {
                if( index > d->text_cursor.blockNumber())
                   d->text_cursor.movePosition(QTextCursor::Down);
                else
                    d->text_cursor.movePosition(QTextCursor::Up);
            }

            //d->textBrowser->setTextCursor(d->text_cursor);
            //qDebug() << "text_cursor.blockNumber()=====" << d->text_cursor.blockNumber();

            //d->text_cursor.movePosition(QTextCursor::StartOfLine);
            d->text_cursor.select(QTextCursor::LineUnderCursor);
            //选中要着色的内容
            fmt.setForeground(QColor(Qt::red));
            d->text_cursor.mergeCharFormat(fmt); //设置文本格式
            d->text_cursor.clearSelection(); //撤销选中

            autoScrollSlot(index);
        }

    }

    void LyricWidget::autoScrollSlot(int num)
    {
        QScrollBar *bar = d->textBrowser->verticalScrollBar();
        int BarValue = d->eachRowHeight * num - d->offSetValue;
        bar->setValue(BarValue);

        //qDebug() << "LyricWidget::autoScrollSlot()@@@@@@" << bar->value() << bar->maximum()
        //    << bar->minimum();
    }

    void LyricWidget::setModelShowTimePos(int )
    {
#if 0
        //model显示的时候设置当前播放歌词
        if(d->timePosIndexMap.find(key) != d->timePosIndexMap.end()) {
            int index = d->timePosIndexMap.value(key);

            qDebug() << "2w2ew2-==================" << posTime << index;
            QModelIndex curModelIndex = d->model->index(index,0);
            d->lyricListView->setCurrentIndex(curModelIndex);
        }
#endif
    }


}