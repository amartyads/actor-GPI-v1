#ifndef REMOTE_CHANNEL_HPP
#define REMOTE_CHANNEL_HPP

#pragma once

#include "connection-type-util.hpp"
#include "Channel.hpp"

#include <vector>
class RemoteChannel: public Channel
{
public:
    void pushData(std::vector<double> &ndata);
    std::vector<double> pullData();
    bool isAvailableToPush();
    bool isAvailableToPull();

    int segmentID;
    int segmentSize;
    
    void initChannel();

    RemoteChannel(ActorConnectionType actorConnType, int segID);
};

#endif