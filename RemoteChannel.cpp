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

RemoteChannel::RemoteChannel(ActorConnectionType actorConnType, int segID, uint64_t source, uint64_t dest, uint64_t remRank)
{
    currConnectionType = actorConnType;
    maxCapacity = 3;
    curCapacity = maxCapacity;
    segmentID = segID;
    std::memcpy(&srcID, &source, sizeof(srcID));
    std::memcpy(&dstID, &dest, sizeof(dstID));
    remoteRank = remRank;

    initChannel();
}
void RemoteChannel::initChannel()
{
    segmentSize = 1010 * sizeof(double);

    const gaspi_segment_id_t tempID = segmentID;
    const gaspi_size_t tempSize = segmentSize;
    ASSERT (gaspi_segment_create(tempID, tempSize
                               , GASPI_GROUP_ALL, GASPI_BLOCK
                               , GASPI_ALLOC_DEFAULT
                               )
         );
    gaspi_pointer_t gasptr_locSeg;
    ASSERT (gaspi_segment_ptr (tempID, &gasptr_locSeg));
	localSegmentPointer = (double*)(gasptr_locSeg);
}
std::vector<double> RemoteChannel::pullData()
{
    if(this->isAvailableToPull() && localSegmentPointer[0] == srcID && localSegmentPointer[1] == dstID)
    {
        std::vector<double> toRet(localSegmentPointer + 3, localSegmentPointer + (int)localSegmentPointer[2]);
        //curCapacity++;
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
        std::vector<double> localCpy {srcID, dstID, (double)ndata.size()};
        localCpy.insert(localCpy.end(), ndata.begin(), ndata.end());
        for(int i = 0; i < localCpy.size();  i++)
        {
            localSegmentPointer[i] = localCpy[i];
        }
        ASSERT (gaspi_write_notify ( segmentID, 0
	                     , remoteRank, segmentID, 0
	                     , segmentSize, 
                         1, 1,
                         1, GASPI_BLOCK
	                     )
	         );
        //data.push(localCpy);
        //curCapacity--;
    }
    else
    {
        throw std::runtime_error("Too much data in channel");
    }
}
bool RemoteChannel::isAvailableToPull()
{
    return gpi_util::test_notif_or_exit(segmentID, 1, 1);
    //return dataStale;
    //return (curCapacity < maxCapacity);
}
bool RemoteChannel::isAvailableToPush()
{
    return true;
    //return (curCapacity > 0);
}