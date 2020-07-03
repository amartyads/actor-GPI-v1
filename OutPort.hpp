#ifndef OUTPORT_HPP
#define OUTPORT_HPP

#include "Channel.hpp"

#pragma once
class OutPort
{
public:
    Channel *connChannel;
    double* data;

    OutPort(Channel* channel)
    {
        connChannel = channel;
    }

    bool isAvailable();         //poll channel to see if space available
    void write(double* ndata);  //write to channel
};

#endif