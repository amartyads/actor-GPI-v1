#ifndef LOCAL_CHANNEL_HPP
#define LOCAL_CHANNEL_HPP

#pragma once

#include "connection-type-util.hpp"
#include "Channel.hpp"
class LocalChannel: public Channel
{
public:
    void pushData(double* ndata);
    double* pullData();
    bool isAvailableToPush();
    bool isAvailableToPull();
    LocalChannel();
};

#endif