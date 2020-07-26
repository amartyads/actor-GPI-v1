#ifndef REMOTE_CHANNEL_HPP
#define REMOTE_CHANNEL_HPP

#pragma once

#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
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

    int blockSize;
    double srcID, dstID;
    double* pushPtr;
    double* pullPtr;
    uint64_t remoteRank;
    std::vector<uint64_t> remoteOffsets;
    
    bool isSender;
    bool isReceiver;
    int queueLocation;
    gaspi_queue_id_t queue_id;

    void initChannel(uint64_t remOffset);

    RemoteChannel(ActorConnectionType actorConnType, int blockSize, int queueLen,
                            bool isSender, bool isReceiver, double* pushPtr,  double* pullPtr, 
                            uint64_t source, uint64_t dest, uint64_t remRank , uint64_t remOffset);
};

#endif