#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "connection-type-util.hpp"
#include "Channel.hpp"
#include "RemoteChannel.hpp"

#include <stdexcept>
#include <queue>
#include <vector>
#include <cstring>
#include <cstdint>

#ifndef ASSERT
#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)
#endif

RemoteChannel::RemoteChannel(ActorConnectionType actorConnType, int blockSize, int queueLen,
                            bool isSender, bool isReceiver, double* pushPtr,  double* pullPtr, 
                            uint64_t source, uint64_t dest, uint64_t remRank , uint64_t remOffset)
{
    currConnectionType = actorConnType;
    maxCapacity = queueLen;
    curCapacity = maxCapacity;
    queueLocation = 0;
    
    this->blockSize = blockSize;
    this->isSender = isSender;
    this->isReceiver = isReceiver;

    this->pushPtr = pushPtr;
    this->pullPtr = pullPtr;

    std::memcpy(&srcID, &source, sizeof(srcID));
    std::memcpy(&dstID, &dest, sizeof(dstID));
    remoteRank = remRank;

    if(isReceiver)
        initChannel(remOffset);
}

void RemoteChannel::initChannel(uint64_t remOffset)
{
    for(int i = 0; i < maxCapacity; i++)
    {
        remoteOffsets.push_back((remOffset * blockSize * maxCapacity * sizeof(double))  + (i * blockSize));
    }
}

std::vector<double> RemoteChannel::pullData()
{   
    //gaspi_printf("In pull\n");
    if(pullPtr[1] == srcID && pullPtr[2] == dstID)
    {
        //gaspi_printf("Size: %d\n",(int)localSegmentPointer[2]);
        std::vector<double> toRet(pullPtr + 3, pullPtr + blockSize);
        //curCapacity++;
        
        pullPtr[0] == 0;
        gpi_util::wait_if_queue_full (1, 1);
        ASSERT (gaspi_write ( 2, 0
                        , remoteRank, 1, remoteOffsets[queueLocation]
                        , sizeof(double), 1, GASPI_BLOCK
                        )
            );//mark as dirty
        
        
        queueLocation++;
        queueLocation %= maxCapacity;
	    ASSERT (gaspi_wait (1, GASPI_BLOCK));
        return toRet;
    }
    else
    {
        throw std::runtime_error("No data in channel");
    }
}
void RemoteChannel::pushData(std::vector<double> &ndata)
{
    
    if(this->isAvailableToPush())
    {
        std::vector<double> localCpy {1, srcID, dstID};
        localCpy.insert(localCpy.end(), ndata.begin(), ndata.end());
        for(int i = 0; i < blockSize;  i++)
        {
            (pushPtr + (blockSize*queueLocation))[i] = localCpy[i];
        }
        //curCapacity--;
        //gaspi_printf("Data pushed\n");
        queueLocation++;
        queueLocation %= maxCapacity;
    }
    else
    {
        throw std::runtime_error("Too much data in channel");
    }
}
bool RemoteChannel::isAvailableToPull()
{
    gpi_util::wait_if_queue_full (1, 1);
    ASSERT (gaspi_read ( 2, 0
                        , remoteRank, 1, remoteOffsets[queueLocation]
                        , blockSize, 1, GASPI_BLOCK
                        )
            );
    ASSERT (gaspi_wait (1, GASPI_BLOCK));
    return (pullPtr[0] == 1);
}
bool RemoteChannel::isAvailableToPush()
{
    return ((pushPtr + (blockSize*queueLocation)) == 0);
}