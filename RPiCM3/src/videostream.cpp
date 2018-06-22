#include <videostream.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

VideoStream::VideoStream()
{
    //gst_init(NULL, NULL);
    if (!fork())
    {
        system("raspivid -v -vf -hf -3d sbs -ih -t 0 -h 730 -w 1280 -md 5 -b 2000000 -fps 45 -o - \
        | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! \
        udpsink host=192.168.168.178 port=5000");
        exit(0);
    }
}