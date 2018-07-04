#include <thetav.h>
#include <array>

ThetaV::ThetaV()
{
}

void ThetaV::takePicture()
{
    if (cameraMode != Mode::PHOTO)
        switchMode(Mode::PHOTO);
    ptp.captureImage();
}

void ThetaV::startVideo()
{
    stopVideo(true);
    if (cameraMode != Mode::VIDEO)
        switchMode(Mode::VIDEO);
    std::array<uint32_t, 5> params = {0,0,0,0,1};
    ptp.genericRequest(0x101c, params);
    videoInProgress = true;
}

void ThetaV::stopVideo(bool force)
{
    if (force)
    {
        std::array<uint32_t, 5> params = {0xFFFFFFFF,0,0,0,0};
        ptp.genericRequest(0x1018, params);
    }
    else if (cameraMode == Mode::VIDEO && videoInProgress)
    {
        std::array<uint32_t, 5> params = {0xFFFFFFFF,0,0,0,0};
        ptp.genericRequest(0x1018, params);
    }
}

void ThetaV::switchMode(Mode mode)
{
    switch (mode) //< pun intended
    {
    case Mode::PHOTO:
        ptp.setProperty(0x5013, "0x0001");
        break;
    case Mode::VIDEO:
        ptp.setProperty(0x5013, "0x8002");
        break;
    case Mode::LIVESTREAM:
        ptp.setProperty(0x5013, "0x8005");
        break;
    default:
        break;
    }
    cameraMode = mode;
}