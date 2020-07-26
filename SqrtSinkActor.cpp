#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include "SqrtSinkActor.hpp"
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>
#include <iostream>

SqrtSinkActor::SqrtSinkActor(std::string name, uint64_t rank, uint64_t srno, int numDigsToGet) : Actor(name, rank, srno)
{
    this->numDigsGot = 0;
    this->numDigsToGet = numDigsToGet;
}

void SqrtSinkActor::act()
{
    for(auto inport : inPortList)
    {
        if(inport -> isAvailable())
        {
            std::vector<double> inData = inport->read();
            std::cout << "Sink received " << inData[0] << std::endl;
            numDigsGot++;
            if(numDigsGot == numDigsToGet)
            {
                finished = true;
            }
        }
    }
}
