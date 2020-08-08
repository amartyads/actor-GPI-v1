#ifndef INPORT_HPP
#define INPORT_HPP

#include "Channel.hpp"
#include <queue>
#include <vector>
#include <string>

#pragma once
class InPort
{
public:
    Channel *connChannel;
    std::vector<double> data;
    std::string name;

    InPort(Channel* channel);
    InPort(Channel* channel, std::string name);

    std::vector<double> read();     // read from channel, return
    bool isAvailable(); // poll channel to see if data available
};

#endif