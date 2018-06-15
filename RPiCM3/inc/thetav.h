#ifndef THETAV_H
#define THETAV_H

#include <ptpcontroller.h>

/// Based on Theta V MTP Spec, available at:
/// https://developers.theta360.com/en/docs/v2/usb_reference/

class ThetaV
{
  public:
    ThetaV();

    /// Value codes for the camera modes
    enum Mode
    {
        LIVESTREAM = 0,
        VIDEO,
        PHOTO
    };

    void takePicture();

    /// Operation code 0x101c with parameters 0,0,1
    void startVideo();

    /// Operation code 0x1018 with parameter 0xFFFFFFFF
    void stopVideo(bool force);

    /// Property code 0x5013
    void switchMode(Mode mode);

  private:
    bool videoInProgress = false;
    Mode cameraMode = Mode::PHOTO;
    PTPController ptp;
};

#endif