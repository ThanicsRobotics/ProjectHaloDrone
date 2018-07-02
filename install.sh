#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "script at: $DIR"
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

if grep -q "bcm2835gpio_swd_nums 20 21" "/usr/local/share/openocd/scripts/interface/raspberrypi2-native.cfg"
then
    echo "interface file already configured"
else
    echo "replacing interface file"
    sudo rm /usr/local/share/openocd/scripts/interface/raspberrypi2-native.cfg
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
    echo 'PATH=/usr/local/gcc-8.1.0/bin:$PATH' >> /etc/profile.d/setenvvars.sh
    cd ..
    echo "Replacing old libstdc++ with new one from GCC 8"
    sudo rm /usr/lib/arm-linux-gnueabihf/libstdc++*
    sudo cp /usr/local/gcc-8.1.0/lib/libstdc++.so.6 /usr/lib/arm-linux-gnueabihf/
fi

if [ -e "/usr/local/lib/libusb-0.1.so.4" ]; then
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
    wget https://sourceforge.net/projects/libptp/files/libptp2/libptp2-1.2.0/libptp2-1.2.0.tar.gz
    tar -xzvf libptp2-1.2.0.tar.gz
    cd libptp2-1.2.0
    ./configure
    make -j4
    sudo make install
    cd ..
fi

if [ -e "/lib/arm-linux-gnueabihf/libncurses.so.5" ]; then
    echo "libncurses already installed"
else
    echo ""
    echo "Installing libncurses"
    echo ""
    sudo apt-get install -y libncurses5-dev libncursesw5-dev
fi

if [ -e "/usr/bin/gst-launch-1.0" ]; then
    echo "libgstreamer already installed"
else
    echo ""
    echo "Installing libgstreamer"
    echo ""
    sudo apt-get install -y libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools
fi

if grep -Fxq "thanics-drone" /etc/hostname
then
    echo 'hostname already set'
else
    echo "Changing hostname to thanics-drone"
    sudo sed -i '1s/.*/thanics-drone/' /etc/hostname
fi

#add raspi-config automation later...

cd $HOME
sudo rm -rf temp

read -p "You need to reboot to make changes, reboot now? (y/n): " choice
if [ $choice == 'y' ]; then
    sudo reboot
fi