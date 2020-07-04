#include "InPort.hpp"
#include "Channel.hpp"

std::vector<double> InPort::read()
{
    return (connChannel->pullData());
}

bool InPort::isAvailable()
{
    return (connChannel->isAvailableToPull());
}