#!/bin/bash
{
    cd /home/pi/HaloController && git pull
} || {
    git clone https://github.com/Legohead259/HaloController.git
    pip install kivy
    pip install kivy-garden
    pip install bcolors
}