#ifndef AUDIOFILEINFO_H
#define AUDIOFILEINFO_H

#include <QObject>
#include <QThread>
#include <QImage>


namespace eink {
	class AudioFileInfoPrivate;

	class AudioFileInfo : public QObject {
		Q_OBJECT
		public :
			AudioFileInfo(QObject *parent = 0);
			~AudioFileInfo();

		public :
		    void startParse();
		    void setFilePath(const std::string &);

		    //get file information
		    std::string getFileName();
		    std::string getFileAuthor();
		    int getFileTimeLenth();
		    QImage getFileCoverImage();


		private :
		    void InitEvn();
		    void openFile();
			void createDecompiler();
			void findVedionStreamType();
			void obtainFileInformation();

		private :
		    AudioFileInfoPrivate *d;

	};
}

#endif
