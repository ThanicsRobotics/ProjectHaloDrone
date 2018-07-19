#include <gimbal.h>
#include <iostream>

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
    GimbalPacket::GetVersion gv;
    std::array<uint8_t, gv.txSize()> txPacket;
    gv.txParse(txPacket);
    serial.write(txPacket.data(), txPacket.size());
    std::array<uint8_t, gv.rxSize()> rxPacket;
    for (int i = 0; i < gv.rxSize(); i++)
    {
        rxPacket[i] = serial.readChar();
    }
    if (gv.checkCrc(rxPacket, (rxPacket[10] << 8 | rxPacket[9])))
    {
        std::cout << "Using STorM32 Gimbal ==========" << "\nFirmware version: " << (rxPacket[4] << 8 | rxPacket[3])
        << "\nSetup layout version: " << (rxPacket[6] << 8 | rxPacket[5]) <<
        "\nBoard capabilities: " << (rxPacket[8] << 8 | rxPacket[7]) 
        << "\n===================================\n" << std::endl;
    }
    else 
    {
        std::cout << "fail\n";
        getVersion();
    }
}