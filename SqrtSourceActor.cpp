#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include "SqrtSourceActor.hpp"
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>
#include <ctime>
#include <iostream>

SqrtSourceActor::SqrtSourceActor(std::string name, uint64_t rank, uint64_t srno, int numDigsToSend) : Actor(name, rank, srno)
{
    this->numDigsSent = 0;
    this->numDigsToSend = numDigsToSend;
    std::srand(std::time(nullptr));
}

void SqrtSourceActor::act()
{
    double curNum = std::rand() % 2000;
    for(auto outport : outPortList)
    {
        if(outport->isAvailable())
        {
            std::cout << "Source sending " << curNum << std::endl;
            std::vector<double> data {curNum};
            outport->write(data);
            numDigsSent++;
            if(numDigsSent == numDigsToSend)
            {
                finished = true;
            }
        }
    }
}
