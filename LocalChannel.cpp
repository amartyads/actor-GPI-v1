#include "connection-type-util.hpp"
#include "Channel.hpp"
#include "LocalChannel.hpp"

#include <stdexcept>
#include <queue>
#include <vector>

LocalChannel::LocalChannel()
{
    currConnectionType = ActorConnectionType::LOCAL_LOCAL;
    maxCapacity = 3;
    curCapacity = maxCapacity;
}
std::vector<double> LocalChannel::pullData()
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
void LocalChannel::pushData(std::vector<double> &ndata)
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
bool LocalChannel::isAvailableToPull()
{
    return (curCapacity < maxCapacity);
}
bool LocalChannel::isAvailableToPush()
{
    return (curCapacity > 0);
}