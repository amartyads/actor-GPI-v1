#include "OutPort.hpp"
#include "Channel.hpp"
#include <string>

OutPort::OutPort(Channel* channel, std::string name)
{
    connChannel = channel;
    this->name = name;
}

OutPort::OutPort(Channel* channel) : OutPort(channel, "temp")  { }

void OutPort::write(std::vector<double> &ndata)
{
    connChannel->pushData(ndata);
}

bool OutPort::isAvailable()
{
    return (connChannel->isAvailableToPush());
}