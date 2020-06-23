#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

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

void Actor::addInPort(InPort* inPort)
{
	inPortList.push_back(inPort);
}
void Actor::addOutPort(OutPort* outPort)
{
	outPortList.push_back(outPort);
}