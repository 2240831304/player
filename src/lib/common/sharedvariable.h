
#ifndef SHAREDVARIABLE_H
#define SHAREDVARIABLE_H

#include <QQueue>
#include <QImage>
#include <QVector>
#include <QList>
#include "struct.h"

//存储解析视频后的每一张图片
extern QList<Vedio_Image_ST> vedioImageQueue_GL;


//播放画面的宽高
extern int playUIWidth_GL;
extern int playUIHeight_GL;
extern bool isPlayAudio_GL;

//当前视频，音频播放时间点，多少毫秒播放一张图片
extern int frameRate_GL;
extern int curVedioPlayTimePos_GL;
extern int curAudioDecoderTimePos_GL;

//音频，视频，数据包是否读完
extern bool isVedioPacketReadFinished_GL;
extern bool isAudioPacketReadFinished_GL;

//音频，视频是否播放完
extern bool isVedioPlayFinished_GL;
extern bool isAudioDecoderFinished_GL;
extern bool isFileHaveAudioStream_GL;

//是否关闭打开的声卡设备
extern bool isCloseAudioDevice_GL;

extern int vedioTotleTimeLenth_GL;

//音乐播放器
extern bool isMusicStartPlay_GL;
extern bool isMusicPlayFinished_GL;
extern int musicPlayTimePos_GL;
extern bool isAutoMusicPlayFinished_GL;

//桌面抓频
extern int desktopGrabPlayWidgetWidth_GL;
extern int desktopGrabPlayWidgetHeight_GL;
extern int SystemScreenWidth_GL;
extern int SystemScreenHeight_GL;


#endif