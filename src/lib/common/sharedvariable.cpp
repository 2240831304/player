

#include <QQueue>
#include <QImage>
#include <QVector>
#include <QList>

#include "struct.h"

//解析完视频数据转化为 图片存储
QList<Vedio_Image_ST> vedioImageQueue_GL;

//播放画面的宽高
int playUIWidth_GL = 0;
int playUIHeight_GL = 0;

//音频同步视频时，音频是否暂停
bool isPlayAudio_GL = false;

//当前视频，音频播放时间点，多少毫秒播放一张图片
int frameRate_GL = 0;
int curVedioPlayTimePos_GL = 0;
int curAudioDecoderTimePos_GL = 0;

//音频，视频，数据包是否读完
bool isVedioPacketReadFinished_GL = false;
bool isAudioPacketReadFinished_GL = false;

//音频，视频是否播放完
bool isVedioPlayFinished_GL = false;
bool isAudioDecoderFinished_GL = false;
bool isFileHaveAudioStream_GL = false;

//是否关闭打开的声卡设备
bool isCloseAudioDevice_GL = false;

//视频的时长
int vedioTotleTimeLenth_GL = 0;


//音乐播放器
bool isMusicStartPlay_GL = false;
bool isMusicPlayFinished_GL = false; //可能切换歌曲，设置状态完成
int musicPlayTimePos_GL = 0;
bool isAutoMusicPlayFinished_GL = false; //歌曲正常播放完

//桌面抓频
int desktopGrabPlayWidgetWidth_GL = 0;
int desktopGrabPlayWidgetHeight_GL = 0;
int SystemScreenWidth_GL = 0;
int SystemScreenHeight_GL = 0;