#ifndef OUTPORT_HPP
#define OUTPORT_HPP

#include "Channel.hpp"

#pragma once
class OutPort
{
public:
    Channel *connChannel;
    std::queue<double**> data;

    OutPort(Channel* channel)
    {
        connChannel = channel;
    }

    void write(double** ndata);
};

#endif