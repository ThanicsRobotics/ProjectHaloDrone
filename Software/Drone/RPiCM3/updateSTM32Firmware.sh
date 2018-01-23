#!/bin/bash

sudo rm /home/pi/STM32Programs/firmware.elf
sudo cp /home/pi/ProjectHalo/Software/Drone/STM32F446/.pioenvs/nucleo_f446re/firmware.elf /home/pi/STM32Programs/
cd /home/pi/STM32Programs/
sudo openocd
