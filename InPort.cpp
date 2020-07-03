#include "InPort.hpp"
#include "Channel.hpp"

double* InPort::read()
{
    return (connChannel->pullData());
}

bool InPort::isAvailable()
{
    return (connChannel->isAvailableToPull());
}