#ifndef INPORT_HPP
#define INPORT_HPP

#include "Channel.hpp"
#include <queue>
#include <vector>

#pragma once
class InPort
{
public:
    Channel *connChannel;
    std::vector<double> data;

    InPort(Channel* channel)
    {
        connChannel = channel;
    }

    std::vector<double> read();     // read from channel, return
    bool isAvailable(); // poll channel to see if data available
};

#endif