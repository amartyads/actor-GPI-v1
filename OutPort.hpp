#ifndef OUTPORT_HPP
#define OUTPORT_HPP

#include "Channel.hpp"

#pragma once
class OutPort
{
public:
    Channel *connChannel;
    double** data;
};

#endif