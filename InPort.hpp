#ifndef INPORT_HPP
#define INPORT_HPP

#include "Channel.hpp"

#pragma once
class InPort
{
public:
    Channel *connChannel;
    double** data;

    InPort(Channel* channel)
    {
        connChannel = channel;
    }
};

#endif