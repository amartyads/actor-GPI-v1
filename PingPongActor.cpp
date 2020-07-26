#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include "PingPongActor.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>
#include <iostream>

PingPongActor::PingPongActor(uint64_t rank, uint64_t srno) : Actor(rank, srno) { }

void PingPongActor::act()
{
	//std::cout << "Actor " << globID << std::endl;
	if(globID == 0) //starter
	{
		if(noTimesRan == 0)
		{
			std::cout << "Actor 0 commencing pingpong" <<std::endl;
			finished = true;
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
			finished = true;
		}
		else
		{
			std::cout << "Actor " << globID << " received nothing yet." << std::endl;
		}
		
	}
	
}