#ifndef STREAM_H
#define STREAM_H

#define VIDEO 0
#define TELE 1

class Stream {
    public:
        Stream(int streamType, char *ip_address, char *port, char *camera_address);
        void closeStream();
        int sendData(char* data);
        int openReceiveChannel();
};

#endif