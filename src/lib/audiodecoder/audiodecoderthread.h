

#include <QObject>
#include <QThread>
#include <QMap>

namespace eink {
    class AudioDecoder;
}

namespace eink{

class AudioDecoderThread : public QThread
{
    Q_OBJECT
    public:
        AudioDecoderThread(QObject *parent = 0);
        ~AudioDecoderThread();

    public:
        void play();
		void stop();
		void suspendPlay();
		void keepOnPlay();
		void adjustPlayTiemPos(int );
		void setFilePath(const std::string &);
        void setParseState(bool);

    private:
        void InitEvn();

    protected:
		void run();

	private :
	    AudioDecoder *audioDecoder;
	    QMap<std::string,AudioDecoder *> audioPlayTypeMap;
	    std::string filePath;

};

}