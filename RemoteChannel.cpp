#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "connection-type-util.hpp"
#include "Channel.hpp"
#include "RemoteChannel.hpp"

#include <stdexcept>
#include <iostream>
#include <queue>
#include <vector>
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

    srcID = (double) source;
    dstID = (double) dest;
    remoteRank = remRank;

    queue_id = 0;

    initChannel(remOffset);
}

void RemoteChannel::initChannel(uint64_t remOffset)
{
    if(isReceiver)
    {
        for(int i = 0; i < maxCapacity; i++)
        {
            remoteOffsets.push_back((remOffset * blockSize * maxCapacity)  + (i * blockSize));
            if(dstID == 1)
                std::cout<<"Offset val " << (remOffset * blockSize * maxCapacity)  + (i * blockSize) <<std::endl;
        }
    }
    if(isSender)
    {
        for(int i = 0; i < maxCapacity; i++)
        {
            (pushPtr + (blockSize*i))[0] = 0;
        }
    }
}

std::vector<double> RemoteChannel::pullData()
{   
    //gaspi_printf("In pull\n");
    //std::cout << std::fixed << srcID << " " <<dstID << std::endl;
    //std::cout << std::fixed << pullPtr[1] << " " <<pullPtr[2] << std::endl;
    if(pullPtr[1] == srcID && pullPtr[2] == dstID)
    {
        //gaspi_printf("Size: %d\n",(int)localSegmentPointer[2]);
        std::vector<double> toRet(pullPtr + 3, pullPtr + blockSize);
        //curCapacity++;
        
        
        pullPtr[0] = 0;
        gpi_util::wait_for_queue_entries(&queue_id, 1);
        ASSERT (gaspi_write ( 2, 0
                        , remoteRank, 1, remoteOffsets[queueLocation]*sizeof(double)
                        , sizeof(double), queue_id, GASPI_BLOCK
                        )
            );//mark as dirty
        
        
        queueLocation++;
        queueLocation %= maxCapacity;
	    ASSERT (gaspi_wait (queue_id, GASPI_BLOCK));
        return toRet;
    }
    else
    {
        throw std::runtime_error("No data in channel");
    }
}
void RemoteChannel::pushData(std::vector<double> &ndata)
{
    if(srcID == 1048576)
        std::cout << "1048576 pushptr" << pushPtr << std::endl;
    if(srcID == 1048577)
        std::cout << "1048577 pushptr" << pushPtr << std::endl;
    //if(this->isAvailableToPush())
    //{
        std::vector<double> localCpy {1, srcID, dstID};
        localCpy.insert(localCpy.end(), ndata.begin(), ndata.end());
        for(int i = 0; i < blockSize;  i++)
        {
            (pushPtr + (blockSize*queueLocation))[i] = localCpy[i];
            if(srcID == 1048577)
                std::cout <<std::fixed<< (pushPtr + (blockSize*queueLocation))[i] << ",";
        }
        //curCapacity--;
        if(srcID == 1048577)
        {
            std::cout << std::endl;
            std::cout << (blockSize * queueLocation) << std::endl;
        }
        //gaspi_printf("Data pushed\n");
        queueLocation++;
        queueLocation %= maxCapacity;
    //}
    //else
    //{
    //    throw std::runtime_error("Too much data in channel");
    //}
}
bool RemoteChannel::isAvailableToPull()
{
    if(srcID == 1048577)
        gaspi_printf("In availabletopull\n");
    gpi_util::wait_for_queue_entries(&queue_id, 1);
    ASSERT (gaspi_read ( 2, 0
                        , remoteRank, 1, remoteOffsets[queueLocation]*sizeof(double)
                        , blockSize * sizeof(double), queue_id, GASPI_BLOCK
                        )
            );
    ASSERT (gaspi_wait (queue_id, GASPI_BLOCK));
    
	//gpi_util::wait_for_flush_queues();
    //if(srcID == 2097153)
    {
        std::cout << std::fixed << pullPtr[0] << " " <<pullPtr[1] << " " <<pullPtr[2] << std::endl;
        //std::cout << queueLocation << std::endl;
        //std::cout << remoteOffsets[queueLocation] << std::endl;
    }
    return (pullPtr[0] == 1);
}
bool RemoteChannel::isAvailableToPush()
{
    //gaspi_printf("In availabletopush\n");
    return ((pushPtr + (blockSize*queueLocation))[0] == 0);
}