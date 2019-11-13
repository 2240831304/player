
#ifndef STRUCT_H
#define STRUCT_H

#include <QString>

struct Vedio_Image_ST {

    void operator = (const Vedio_Image_ST &other)
    {
        image = other.image;
        playTimePos = other.playTimePos;
        isKeyFrame = other.isKeyFrame;
        subtitleContent = other.subtitleContent;
    }


    QImage image;
    int playTimePos;
    bool isKeyFrame; //是否关键帧
    QString subtitleContent; //字幕内容
};

struct File_Information_ST {

    void operator = (const File_Information_ST &other)
    {
        fileName = other.fileName;
        fileAuthor = other.fileAuthor;
        timeLenth = other.timeLenth;
        coverImage = other.coverImage;
    }

    std::string fileName;
    std::string fileAuthor;
    int timeLenth;
    QImage coverImage;
};

#endif