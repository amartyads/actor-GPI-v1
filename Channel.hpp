#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#pragma once

#include "connection-type-util.hpp"
#include <queue>
class Channel
{
public:
    ActorConnectionType currConnectionType;
    int maxCapacity, curCapacity;
    std::queue<double* > data;

    virtual void pushData(double* ndata) = 0;
    virtual double* pullData() = 0;
    virtual bool isAvailableToPush() = 0;
    virtual bool isAvailableToPull() = 0;
};

#endif