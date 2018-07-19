#ifndef GIMBAL_H
#define GIMBAL_H

#include <serial.h>
#include <gimbalpackets.h>

class Gimbal
{
public:
    Gimbal();
    void getVersion();

private:
    Serial serial;

    void setup();
};

#endif