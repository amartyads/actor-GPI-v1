#ifndef LOCAL_CHANNEL_HPP
#define LOCAL_CHANNEL_HPP

#pragma once

#include "connection-type-util.hpp"
#include "Channel.hpp"

#include <vector>
class LocalChannel: public Channel
{
public:
    void pushData(std::vector<double> &ndata);
    std::vector<double> pullData();
    bool isAvailableToPush();
    bool isAvailableToPull();
    LocalChannel();
};

#endif