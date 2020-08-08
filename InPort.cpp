#include "InPort.hpp"
#include "Channel.hpp"
#include <string>

InPort::InPort(Channel* channel, std::string name)
{
    connChannel = channel;
    this->name = name;
}

InPort::InPort(Channel* channel) : InPort(channel, "temp")  { }

std::vector<double> InPort::read()
{
    return (connChannel->pullData());
}

bool InPort::isAvailable()
{
    return (connChannel->isAvailableToPull());
}