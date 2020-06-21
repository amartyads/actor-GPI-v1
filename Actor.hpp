#ifndef ACTOR_HPP
#define ACTOR_HPP


#include <stdlib.h>
#include <utility>
#include <vector>
#include <string>

#pragma once

#include "InPort.hpp"
#include "OutPort.hpp"


class Actor
{
public:
	std::string name;
	int rank;
	int srno;
	int globID;
	int noTimesRan;

	std::vector<InPort* > inPortList;
	std::vector<OutPort* > outPortList;

	void addInPort(InPort* inPort);
	void addOutPort(OutPort* outPort);

	Actor(std::string othname, int othrank, int othsrno)
	{
		name = othname;
		rank = othrank;
		srno = othsrno;
		globID = (rank << 10) | srno;
		noTimesRan = 0;
	}
	Actor(int rank, int srno) : Actor("A-"+std::to_string(rank)+"-"+std::to_string(srno), rank, srno) { }

	static int encodeGlobID(int procNo, int actNo);
	static std::pair<int,int> decodeGlobID(int inpGlobId);

};

#endif