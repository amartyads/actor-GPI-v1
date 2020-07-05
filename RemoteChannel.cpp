#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "connection-type-util.hpp"
#include "Channel.hpp"
#include "RemoteChannel.hpp"

#include <stdexcept>
#include <queue>
#include <vector>

RemoteChannel::RemoteChannel(ActorConnectionType actorConnType, int segID)
{
    currConnectionType = actorConnType;
    maxCapacity = 3;
    curCapacity = maxCapacity;
    segmentID = segID;
    initChannel();
}
void RemoteChannel::initChannel()
{

}
std::vector<double> RemoteChannel::pullData()
{
    if(this->isAvailableToPull())
    {
        std::vector<double> toRet = data.front();
        data.pop();
        curCapacity++;
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
        std::vector<double> localCpy(ndata);
        data.push(localCpy);
        curCapacity--;
    }
    else
    {
        throw std::runtime_error("Too much data in channel");
    }
}
bool RemoteChannel::isAvailableToPull()
{
    return (curCapacity < maxCapacity);
}
bool RemoteChannel::isAvailableToPush()
{
    return (curCapacity > 0);
}