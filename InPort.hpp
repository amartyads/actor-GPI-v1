#ifndef INPORT_HPP
#define INPORT_HPP

#include "Channel.hpp"
#include <queue>

#pragma once
class InPort
{
public:
    Channel *connChannel;
    double* data;

    InPort(Channel* channel)
    {
        connChannel = channel;
    }

    double* read();     // read from channel, return
    bool isAvailable(); // poll channel to see if data available
};

#endif