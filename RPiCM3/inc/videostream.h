#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <string>
#include <thread>
#include <gst/gst.h>
#include <types.h>

struct ReceiveAddress {
    std::string ip = "";
    std::string port = "";
};

struct VideoSettings {
    int camera = 2;
    int bitrate = 2000000;
    bool vFlip = true;
    bool hFlip = true;
    int height = 900;
    int width = 896;
    int fps = 40;

    bool record = false;
    bool stream = false;

    ReceiveAddress addr;
};

class VideoStream {
public:
    VideoStream();
    VideoStream(VideoSettings& videoSettings);
    ~VideoStream();

    struct PipelineData {
        gboolean is_live = false;
        GstElement *pipeline = nullptr;
        GMainLoop *loop = nullptr;
    };

    void startPipeline();
    void stopPipeline();

private:
    bool pipelineStarted = false;

    VideoSettings settings;
    std::string videoStorageDirectory = "/home/pi/HaloVideos/";
    std::thread pipelineThread;
    PipelineData data;

    void startPipelineInThread();
    //void callbackMessage(GstBus *bus, GstMessage *msg);
};

#endif