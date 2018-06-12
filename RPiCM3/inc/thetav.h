#ifndef THETAV_H
#define THETAV_H

#include <ptpcontroller.h>

/// Based on Theta V MTP Spec, available at:
/// https://developers.theta360.com/en/docs/v2/usb_reference/

class ThetaV
{
  public:
    /// Value codes for the camera modes
    enum MODE
    {
        LIVESTREAM = 0x8005,
        VIDEO = 0x8002,
        PHOTO = 0x0001
    };

    void takePicture();

    /// Operation code 0x101c with parameters 0,0,1
    void startVideo();

    /// Operation code 0x1018 with parameter 0xFFFFFFFF
    void stopVideo();

    /// Property code 0x5013
    void switchMode(int mode);

  private:
    bool videoInProgress = false;
    PTPController ptp;
};

#endif