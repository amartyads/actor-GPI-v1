#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

#include <iostream>

Actor::Actor(std::string othname, uint64_t othrank, uint64_t othsrno)
{
	name = othname;
	rank = othrank;
	srno = othsrno;
	globID = Actor::encodeGlobID(othrank, othsrno);
	noTimesRan = 0;
	receivedData = false;
}
Actor::Actor(uint64_t rank, uint64_t srno) : Actor("A-"+std::to_string(rank)+"-"+std::to_string(srno), rank, srno) { }

Actor::~Actor()
{
	for(auto ip : this-> inPortList)
		delete ip;
	for(auto op : this-> outPortList)
		delete op;
}

void Actor::addInPort(InPort* inPort)
{
	inPortList.push_back(inPort);
}
void Actor::addOutPort(OutPort* outPort)
{
	outPortList.push_back(outPort);
}

uint64_t Actor::encodeGlobID(uint64_t procNo, uint64_t actNo) //static
{
	return (procNo << 20) | actNo;
}

std::pair<uint64_t,uint64_t> Actor::decodeGlobID(uint64_t inpGlobId) //static
{
	uint64_t procNo = inpGlobId >> 20;
	uint64_t actNo = inpGlobId & ((1 << 20) - 1);
	return (std::make_pair(procNo, actNo));
}