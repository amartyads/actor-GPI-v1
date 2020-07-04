#ifndef OUTPORT_HPP
#define OUTPORT_HPP

#include "Channel.hpp"

#include <vector>

#pragma once
class OutPort
{
public:
    Channel *connChannel;
    std::vector<double> data;

    OutPort(Channel* channel)
    {
        connChannel = channel;
    }

    bool isAvailable();         //poll channel to see if space available
    void write(std::vector<double> &ndata);  //write to channel
};

#endif