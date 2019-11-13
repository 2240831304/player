#ifndef VEDIODECODER_H
#define VEDIODECODER_H

enum PLAYER_TYPE {
	MP4PLAYER

};

const std::string PLAYER_MSG[] = {
	"mp4"
};

namespace eink {

    class VedioDecoder {
        public :
            virtual	~VedioDecoder() {}

            virtual void play()=0;
            virtual void stop()=0;
            virtual void suspendPlay()=0;
            virtual void keepOnPlay()=0;
            virtual void adjustPlayTiemPos(int ) =0;
            virtual const char *getVedioPlayerType()=0;
            virtual void setFilePath(const std::string &)=0;
            virtual void setParseState(bool)=0;

    };

}

#endif
