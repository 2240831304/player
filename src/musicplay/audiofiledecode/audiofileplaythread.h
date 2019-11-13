

#include <QObject>
#include <QThread>
#include "audiofiledecoder.h"

class AudioFilePlayThread : public QThread
{
    Q_OBJECT
    public:
        AudioFilePlayThread(QObject *parent = 0);
        ~AudioFilePlayThread();

        void setAudioFilePlayer( AudioFileDecoder *);

    protected:
		void run();

	private :
	    AudioFileDecoder *audioPlayer;

};
