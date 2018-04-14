#ifndef STREAM_H
#define STREAM_H

#include <string.h>
#include <iostream>

extern std::string camera;
extern std::string receiver;

void startStream(std::string camera, std::string address);

#endif