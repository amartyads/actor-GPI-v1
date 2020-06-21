#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#pragma once

#include "connection-type-util.hpp"
class Channel
{
public:
    ActorConnectionType currConnectionType;

    virtual void pushData() = 0;
    virtual void pullData() = 0;
};

#endif