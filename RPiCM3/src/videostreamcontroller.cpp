#include <videostreamcontroller.h>
#include <gst/gst.h>

VideoStreamController::VideoStreamController()
    : mStreamLeft(VideoStream(mSettings[0])), mStreamRight(VideoStream(mSettings[1]))
{

}

VideoStreamController::~VideoStreamController()
{

}

void VideoStreamController::setFormat(VideoFormat cameraFormat)
{
    mCameraFormat = cameraFormat;
}

void VideoStreamController::setSettings(std::array<VideoSettings, 2>& settings)
{
    mSettings = settings;
    mStreamLeft.setSettings(mSettings[0]);
    mStreamRight.setSettings(mSettings[1]);
}

void VideoStreamController::start()
{   
    gst_init(NULL, NULL);
    mStreamLeft.startPipeline();
    if (mCameraFormat == VideoFormat::STEREO)
    {
        mStreamRight.startPipeline();
    }
}