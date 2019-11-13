
#include <QWidget>
#include <QObject>
#include <QStringList>

class QStandardItemModel;

namespace eink {
    class  ListView;
}

enum FILE_TYPE {
    FILE_AUDIO,
    FILE_VEDIO,
    FILE_IMAGE,
    FILE_BOOK
};

class FileManagerView : public QWidget
{
    Q_OBJECT
    public:
        FileManagerView(QWidget *parent = 0);
        ~FileManagerView();

        void setFileType(FILE_TYPE );
        void sendNextFileMes();
        void sendLastFileMes();
        QString getNextFilePath();
        QString getLastFilePath();

    private:
        void setupUI();
        void addItemData(const QString &);

    signals:
        void doubleClicked(const QModelIndex &);

    private slots:
        void addFileSlot();
        void doubleClickedSlot(const QModelIndex &);

    private:
        QStandardItemModel *model;
        FILE_TYPE fileType;
        QStringList fileStrList;
        eink::ListView *fileListView;

};