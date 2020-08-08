#ifndef OUTPORT_HPP
#define OUTPORT_HPP

#include "Channel.hpp"

#include <vector>
#include <string>

#pragma once
class OutPort
{
public:
    Channel *connChannel;
    std::vector<double> data;
    std::string name;

    OutPort(Channel* channel);
    OutPort(Channel* channel, std::string name);

    bool isAvailable();         //poll channel to see if space available
    void write(std::vector<double> &ndata);  //write to channel
};

#endif