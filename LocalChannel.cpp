#include "connection-type-util.hpp"
#include "Channel.hpp"
#include "LocalChannel.hpp"

#include <stdexcept>

LocalChannel::LocalChannel()
{
    currConnectionType = ActorConnectionType::LOCAL_LOCAL;
    maxCapacity = 3;
    curCapacity = maxCapacity;
}
double* LocalChannel::pullData()
{
    if(this->isAvailableToPull())
    {
        double* toRet = data.front();
        data.pop();
        curCapacity++;
    }
    else
    {
        throw std::runtime_error("No data in channel");
    }
    
}
void LocalChannel::pushData(double* ndata)
{
    if(this->isAvailableToPush())
    {
        data.push(ndata);
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