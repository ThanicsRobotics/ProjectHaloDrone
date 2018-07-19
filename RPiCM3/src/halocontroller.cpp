#include <halocontroller.h>
#include <wiringPi.h>

HaloController::HaloController(std::shared_ptr<bool> shutdown, CommandLineOptions& clo)
    : shutdownIndicator(shutdown), fc(shutdownIndicator)
{
    VideoFormat format;
    if (clo.ports[0].empty() && clo.ports[1].empty() && 
        clo.record == VideoFormat::NONE) format = VideoFormat::NONE;
    else if ((!clo.ports[0].empty() && !clo.ports[1].empty()) || clo.record == VideoFormat::STEREO)
        format = VideoFormat::STEREO;
    else format = VideoFormat::MONO;

    mVideoSettings[0].camera = 0;
    mVideoSettings[0].bitrate = 2000000;
    mVideoSettings[0].vFlip = true;
    mVideoSettings[0].hFlip = true;
    mVideoSettings[0].height = 900;
    mVideoSettings[0].width = 896;
    mVideoSettings[0].fps = 40;

    mVideoSettings[0].record = (clo.record > VideoFormat::NONE) ? true : false;
    mVideoSettings[0].stream = !clo.ports[0].empty();
    mVideoSettings[0].addr.ip = clo.ipAddress;
    mVideoSettings[0].addr.port = clo.ports[0];

    mVideoSettings[1].camera = 1;
    mVideoSettings[1].bitrate = 2000000;
    mVideoSettings[1].vFlip = true;
    mVideoSettings[1].hFlip = true;
    mVideoSettings[1].height = 900;
    mVideoSettings[1].width = 896;
    mVideoSettings[1].fps = 40;

    mVideoSettings[1].record = (clo.record == VideoFormat::STEREO) ? true : false;
    mVideoSettings[1].stream = !clo.ports[1].empty();
    mVideoSettings[1].addr.ip = clo.ipAddress;
    mVideoSettings[1].addr.port = clo.ports[1];

    vsc.setSettings(mVideoSettings);
    vsc.setFormat(format);
}

HaloController::~HaloController()
{

}

void HaloController::startVideoPipelines()
{
    vsc.start();
}

void HaloController::startFlightController()
{
    fc.startFlight();
}