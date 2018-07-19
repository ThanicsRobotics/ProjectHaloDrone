#include <gimbal.h>
#include <iostream>
#include <wiringPi.h>

Gimbal::Gimbal()
    : serial("/dev/ttyS0", 115200)
{
    setup();
}

void Gimbal::setup()
{
    getVersion();
}

void Gimbal::getVersion()
{
    GimbalPacket::GetVersion::Packet gv;
    std::array<uint8_t, gv.txPacketSize()> txPacket;
    gv.txParse(txPacket);
    serial.write(txPacket.data(), txPacket.size());
    std::array<uint8_t, gv.rxPacketSize()> rxPacket;
    for (int i = 0; i < gv.rxPacketSize(); i++)
    {
        rxPacket[i] = serial.readChar();
    }
    if (gv.isCrcGood(rxPacket))
    {
        std::cout << "\nUsing STorM32 Gimbal ==============" << "\nFirmware version: " 
        << (rxPacket[4] << 8 | rxPacket[3])
        << "\nSetup layout version: " << (rxPacket[6] << 8 | rxPacket[5])
        << "\nBoard capabilities: " << (rxPacket[8] << 8 | rxPacket[7]) 
        << "\n===================================\n" << std::endl;
    }
    else
    {
        std::cout << "fail\n";
        getVersion();
    }
}

AngularPosition Gimbal::getAngularPosition()
{
    

}

void Gimbal::setAngularPosition(const AngularPosition& position)
{
    std::cout << "Setting Gimbal position to:\n"
    << "Pitch: " << (int)position.pitch
    << "\nRoll: " << (int)position.roll
    << "\nYaw: " << (int)position.yaw << std::endl;
    GimbalPacket::SetAngles::Packet sa;
    std::array<uint8_t, sa.txPacketSize()> txPacket;
    sa.txParse(txPacket, position.pitch, position.roll, position.yaw);
    serial.write(txPacket.data(), txPacket.size());
    std::array<uint8_t, sa.rxPacketSize()> rxPacket;
    for (int i = 0; i < sa.rxPacketSize(); i++)
    {
        rxPacket[i] = serial.readChar();
    }
    if (sa.isCrcGood(rxPacket)) 
    {
        std::cout << "CRC good\n";
    }
    else
    {
        std::cout << "CRC bad, retrying\n";
        setAngularPosition(position);
    }
    std::cout << "Setting complete\n";
}