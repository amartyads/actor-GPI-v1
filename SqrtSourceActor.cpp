#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include "SqrtSourceActor.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>
#include <iostream>

SqrtSourceActor::SqrtSourceActor(std::string name, uint64_t rank, uint64_t srno, int numDigsToSend) : Actor(name, rank, srno)
{
    this->numDigsSent = 0;
    this->numDigsToSend = numDigsToSend;
}
