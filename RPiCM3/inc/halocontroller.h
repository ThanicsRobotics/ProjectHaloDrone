#ifndef HALOCONTROLLER_H
#define HALOCONTROLLER_H

#include <flightcontroller.h>
#include <videostreamcontroller.h>
#include <thetav.h>
#include <types.h>
#include <memory>
#include <array>

class HaloController
{
public:
    HaloController(std::shared_ptr<bool> shutdown, CommandLineOptions& clo);
    ~HaloController();

    void startVideoPipelines();
    void startFlightController();

private:
    std::shared_ptr<bool> shutdownIndicator;

    FlightController fc;
    VideoStreamController vsc;
    ThetaV mThetaV;
    std::array<VideoSettings, 2> mVideoSettings;
};

#endif