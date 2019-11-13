#ifndef AUDIOFILEDECODER_H
#define AUDIOFILEDECODER_H

enum AUDIO_FILE_TYPE {
	FILE_MP4,
	FILE_MP3,
	FILE_WAV,
	FILE_FLAC

};

const std::string AUDIO_MSG[] = {
	"mp4",
	"mp3",
	"wav",
	"flac"
};

class AudioFileDecoder {
	public :
	    virtual	~AudioFileDecoder() {}

		virtual void play()=0;
		virtual void stop()=0;
		virtual void suspendPlay()=0;
		virtual void keepOnPlay()=0;
		virtual const char *getAudioPlayerType()=0;
		virtual void setFilePath(const std::string &)=0;
		virtual bool getPlayFinishedState()=0;
		virtual void adjustPlayTimePos(int )=0;

};

#endif
