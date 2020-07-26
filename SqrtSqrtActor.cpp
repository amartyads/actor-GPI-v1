#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include "SqrtSqrtActor.hpp"
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>
#include <cmath>
#include <iostream>

SqrtSqrtActor::SqrtSqrtActor(std::string name, uint64_t rank, uint64_t srno, int numDigsToSend) : Actor(name, rank, srno)
{
    this->numDigsSent = 0;
    this->numDigsToSend = numDigsToSend;
    writeSuccess = true;
}

void SqrtSqrtActor::act()
{
    if(writeSuccess)
    {
        bool hasInData;
        int i;
        for(i = 0; i < inPortList.size(); i++)
        {
            if(inPortList[i]->isAvailable())
            {
                hasInData = true;
                break;
            }
        }
        if(hasInData)
        {
            inData = inPortList[i]->read();
            std::cout << "Rooter received: " << inData[0];
            double temp = std::sqrt(inData[0]);
            std::cout << "Sending value: " << temp << std::endl;
            writeSuccess = false;
            inData[0] = temp;
            for(auto outport: outPortList)
            {
                if(outport->isAvailable())
                {
                    writeSuccess = true;
                    outport->write(inData);

                    numDigsSent++;
                    if(numDigsSent == numDigsToSend)
                    {
                        finished = true;
                    }
                }
            }
        }
        
    }
    else
    {
        for(auto outport: outPortList)
        {
            if(outport->isAvailable())
            {
                writeSuccess = true;
                outport->write(inData);

                numDigsSent++;
                if(numDigsSent == numDigsToSend)
                {
                    finished = true;
                }
            }
        }
    }
}
