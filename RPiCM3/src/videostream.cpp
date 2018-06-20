#include <videostream.h>

#include <stdio.h>
#include <gst/gst.h>

VideoStream::VideoStream()
{
    gst_init(NULL, NULL);
}