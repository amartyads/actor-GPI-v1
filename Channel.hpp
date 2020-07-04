#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#pragma once

#include "connection-type-util.hpp"
#include <queue>
#include <vector>
class Channel
{
public:
    ActorConnectionType currConnectionType;
    int maxCapacity, curCapacity;
    std::queue< std::vector<double> > data;

    virtual void pushData(std::vector<double> &ndata) = 0;
    virtual std::vector<double> pullData() = 0;
    virtual bool isAvailableToPush() = 0;
    virtual bool isAvailableToPull() = 0;
};

#endif