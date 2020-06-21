#include "Actor.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>

int Actor::encodeGlobID(int procNo, int actNo) //static
{
	return (procNo << 10) | actNo;
}

std::pair<int,int> Actor::decodeGlobID(int inpGlobId) //static
{
	int procNo = inpGlobId >> 10;
	int actNo = inpGlobId & ((1 << 10) - 1);
	return *(new std::pair<int, int>(actNo, procNo));
}

void Actor::addInPort(InPort* inPort)
{
	inPortList.push_back(inPort);
}
void Actor::addOutPort(OutPort* outPort)
{
	outPortList.push_back(outPort);
}