#ifndef GIMBAL_H
#define GIMBAL_H

#include <serial.h>
#include <gimbalpackets.h>
#include <types.h>

class Gimbal
{
public:
    Gimbal();
    void getVersion();
    AngularPosition getAngularPosition();
    void setAngularPosition(const AngularPosition& position);

private:
    Serial serial;

    void setup();
};

#endif