#include "OutPort.hpp"
#include "Channel.hpp"

void OutPort::write(double* ndata)
{
    connChannel->pushData(ndata);
}

bool OutPort::isAvailable()
{
    return (connChannel->isAvailableToPush());
}