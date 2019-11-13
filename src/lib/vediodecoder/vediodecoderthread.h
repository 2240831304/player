
#include <QObject>
#include <QThread>
#include <QImage>
#include <QMap>

namespace eink{

class VedioDecoder;

class VedioDecoderThread : public QThread
{
    Q_OBJECT

	public:
		VedioDecoderThread(QObject *parent = 0);
		~VedioDecoderThread();

		void play();
        void stop();
        void suspendPlay();
        void keepOnPlay();
        void adjustPlayTiemPos(int ) ;
        void setFilePath(const std::string &);
        void setParseState(bool);

    private:
        void InitEvn();

	protected:
		void run();

	private :
		std::string fileDir;
		VedioDecoder *curVedioDecoder;
		QMap<std::string,VedioDecoder *> vedioDecoderMap;

};

}
