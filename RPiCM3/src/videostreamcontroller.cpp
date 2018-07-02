#include <videostreamcontroller.h>

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
}

void VideoStreamController::start()
{
    //mStreams.push_back(VideoStream(mSettings[0]));
    mStreamLeft.startPipeline();
    if (mCameraFormat == VideoFormat::STEREO)
    {
        //mStreams.push_back(VideoStream(mSettings[1]));
        mStreamRight.startPipeline();
    }
}