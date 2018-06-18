#!/bin/bash

# echo ""
# echo "Downloading Thanics Halo Drone repository to $HOME"
# cd $HOME
# git clone https://github.com/ThanicsRobotics/ProjectHaloDrone.git

cd $HOME
mkdir temp
cd temp

if [ -e "/usr/local/lib/libpigpio.so" ]; then
    echo "PIGPIO already installed"
else
    echo ""
    echo "Downloading and installing PIGPIO"
    echo ""
    wget abyz.me.uk/rpi/pigpio/pigpio.zip
    unzip pigpio.zip
    cd PIGPIO
    make -j4
    sudo make install
    cd ..
fi

if [ -e "/usr/local/lib/libwiringPi.so" ]; then
    echo "wiringPi already installed"
else
    echo ""
    echo "Downloading and installing wiringPi"
    echo ""
    git clone git://git.drogon.net/wiringPi
    cd wiringPi
    sudo ./build
    cd ..
fi

if [ -d "/usr/local/share/openocd" ]; then
    echo "OpenOCD already installed"
else
    echo ""
    echo "Installing OpenOCD dependencies"
    echo ""
    sudo apt-get install -y git autoconf libtool make pkg-config libusb-1.0-0 libusb-1.0-0-dev
    echo ""
    echo "Downloading and installing OpenOCD"
    echo ""
    git clone git://git.code.sf.net/p/openocd/code openocd-code
    cd openocd-code
    ./bootstrap
    ./configure --enable-sysfsgpio --enable-bcm2835gpio
    make -j4
    sudo make install
    cd ..
fi

if grep -Fxq "bcm2835gpio_swd_nums 20 21" /usr/local/share/openocd/scripts/interface/raspberrypi2-native.cfg
then
    echo "interface file already configured"
else
    echo "replacing interface file"
    sudo rm /usr/local/share/openocd/scripts/interface/raspberrypi2-native.cfg
    DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    sudo cp $DIR/install/raspberrypi2-native.cfg /usr/local/share/openocd/scripts/interface/
fi

if [ -e "/usr/local/gcc-8.1.0/bin/g++-8.1.0" ]; then
    echo "GCC version 8 already installed"
else
    echo ""
    echo "Installing GCC version 8"
    echo ""
    git clone https://bitbucket.org/sol_prog/raspberry-pi-gcc-binary.git
    cd raspberry-pi-gcc-binary
    tar xf gcc-8.1.0.tar.bz2
    sudo mv gcc-8.1.0 /usr/local
    sudo cp /usr/local/gcc-8.1.0/bin/g++-8.1.0 /usr/local/bin
    sudo cp /usr/local/gcc-8.1.0/bin/gcc-8.1.0 /usr/local/bin
    cd ..
    echo "Replacing old libstdc++ with new one from GCC 8"
    sudo rm /usr/lib/arm-linux-gnueabihf/libstdc++*
    sudo cp /usr/local/gcc-8.1.0/lib/libstdc++.so.6 /usr/lib/arm-linux-gnueabihf/
fi

if [ -e "/usr/local/lib/libptp2.so.4" ]; then
    echo "libusb already installed"
else
    echo ""
    echo "Installing legacy libusb (needed by libptp)"
    echo ""
    wget https://sourceforge.net/projects/libusb/files/libusb-0.1%20%28LEGACY%29/0.1.12/libusb-0.1.12.tar.gz
    tar xzvf libusb-0.1.12.tar.gz
    cd libusb-0.1.12/
    ./configure
    make -j4
    sudo make install
fi

if [ -e "/usr/local/lib/libptp2.so" ]; then
    echo "libptp2 already installed"
else
    echo ""
    echo "Installing libptp2"
    echo ""

if [ -e "/usr/local/lib/libptp2.so" ]; then
    echo "libptp2 already installed"
else
    echo ""
    echo "Installing libptp2"
    echo ""
    wget https://sourceforge.net/projects/libptp/files/libptp2/libptp2-1.2.0/libptp2-1.2.0.tar.gz
    tar -xzvf libptp2-1.2.0.tar.gz
    cd libptp2-1.2.0
    ./configure
    make -j4
    sudo make install
    cd ..
fi

cd $HOME
rm -r temp
