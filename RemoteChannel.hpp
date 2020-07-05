#ifndef REMOTE_CHANNEL_HPP
#define REMOTE_CHANNEL_HPP

#pragma once

#include "connection-type-util.hpp"
#include "Channel.hpp"

#include <vector>
#include <cstdint>
class RemoteChannel: public Channel
{
public:
    void pushData(std::vector<double> &ndata);
    std::vector<double> pullData();
    bool isAvailableToPush();
    bool isAvailableToPull();

    int segmentID;
    int segmentSize;
    double srcID, dstID;
    double* localSegmentPointer;
    uint64_t remoteRank;
    
    void initChannel();

    RemoteChannel(ActorConnectionType actorConnType, int segID, uint64_t source, uint64_t dest, uint64_t remRank);
};

#endif