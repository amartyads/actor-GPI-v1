#ifndef INPORT_HPP
#define INPORT_HPP

#include "Channel.hpp"
#include <queue>

#pragma once
class InPort
{
public:
    Channel *connChannel;
    std::queue<double**> data;

    InPort(Channel* channel)
    {
        connChannel = channel;
    }

    double** read();
    bool isAvailable();
};

#endif