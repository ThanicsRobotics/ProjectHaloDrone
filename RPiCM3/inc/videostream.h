#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <string>
#include <thread>
#include <gst/gst.h>

class VideoStream {
public:
    struct VideoSettings {
        int bitrate = 2000000;
        bool vFlip = true;
        bool hFlip = true;
        int height = 900;
        int width = 896;
        int fps = 40;
    };

    struct ReceiveAddress {
        std::string ip = "192.168.168.178";
        std::string port = "5002";
    };
    struct PipelineData {
        gboolean is_live = false;
        GstElement *pipeline = nullptr;
        GMainLoop *loop = nullptr;
    };

    VideoStream(std::shared_ptr<bool> shutdown, VideoSettings videoSettings,
        bool doRecord, int camera, ReceiveAddress address);
    ~VideoStream();
    void startPipeline();
    void stopPipeline();

private:
    bool pipelineStarted = false;

    VideoSettings settings;
    std::shared_ptr<bool> shutdownIndicator;
    bool record = false;
    int cameraNumber = 0;
    ReceiveAddress addr;
    std::string videoStorageDirectory = "/home/pi/HaloVideos/";
    std::thread pipelineThread;
    PipelineData data;

    void startPipelineInThread();
    //void callbackMessage(GstBus *bus, GstMessage *msg);
};

#endif