#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

#include <iostream>

void Actor::addInPort(InPort* inPort)
{
	inPortList.push_back(inPort);
}
void Actor::addOutPort(OutPort* outPort)
{
	outPortList.push_back(outPort);
}

void Actor::act()
{
	//std::cout << "Actor " << globID << std::endl;
	if(globID == 0) //starter
	{
		if(noTimesRan == 0)
		{
			std::cout << "Actor 0 commencing pingpong" <<std::endl;
			receivedData = true;
			std::vector<double> data {42.42};
			for(int j = 0; j < outPortList.size(); j++)
			{
				if(outPortList[j]->isAvailable())
					outPortList[j]->write(data);
			}
			noTimesRan++;
		}
		else
		{
			std::cout << "Actor 0 already commenced pingpong" <<std::endl;
		}
		
	}
	else
	{
		bool hasInData = false;
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
			std::vector<double> data = inPortList[i]->read();
			std::cout << "Actor " << globID << " received "<< data[0] << std::endl;
			data[0] = data[0] + 10.0;
			for(int j = 0; j < outPortList.size(); j++)
			{
				if(outPortList[j]->isAvailable())
					outPortList[j]->write(data);
			}
			receivedData = true;
		}
		else
		{
			std::cout << "Actor " << globID << " received nothing yet." << std::endl;
		}
		
	}
	
}



uint64_t Actor::encodeGlobID(uint64_t procNo, uint64_t actNo) //static
{
	return (procNo << 20) | actNo;
}

std::pair<uint64_t,uint64_t> Actor::decodeGlobID(uint64_t inpGlobId) //static
{
	uint64_t procNo = inpGlobId >> 20;
	uint64_t actNo = inpGlobId & ((1 << 20) - 1);
	return (std::make_pair(actNo, procNo));
}