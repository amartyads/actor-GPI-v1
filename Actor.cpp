#include "Actor.hpp"
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