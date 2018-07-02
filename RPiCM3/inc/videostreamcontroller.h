#ifndef VIDEOSTREAMCONTROLLER_H
#define VIDEOSTREAMCONTROLLER_H

#include <types.h>
#include <videostream.h>
#include <vector>
#include <array>

class VideoStreamController
{
public:
    VideoStreamController();
    ~VideoStreamController();
    void setFormat(VideoFormat cameraFormat);
    void setSettings(std::array<VideoSettings, 2>& settings);
    void start();

private:
    std::array<VideoSettings, 2> mSettings;
    VideoFormat mCameraFormat;

    VideoStream mStreamLeft;
    VideoStream mStreamRight;
};

#endif