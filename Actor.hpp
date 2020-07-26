#ifndef ACTOR_HPP
#define ACTOR_HPP


#include <stdlib.h>
#include <utility>
#include <vector>
#include <string>
#include <cstdint>

#pragma once

#include "InPort.hpp"
#include "OutPort.hpp"


class Actor
{
public:
	std::string name;
	uint64_t rank;
	uint64_t srno;
	uint64_t globID;
	int noTimesRan;

	std::vector<InPort* > inPortList;
	std::vector<OutPort* > outPortList;

	void addInPort(InPort* inPort);
	void addOutPort(OutPort* outPort);

	Actor(std::string othname, uint64_t othrank, uint64_t othsrno);
	Actor(uint64_t rank, uint64_t srno);

	virtual ~Actor();
	Actor(Actor &other) = delete;
	Actor & operator=(Actor &other) = delete;
	//temps
	virtual void act() = 0;
	bool finished;
	//statics
	static uint64_t encodeGlobID(uint64_t procNo, uint64_t actNo);
	static std::pair<uint64_t,uint64_t> decodeGlobID(uint64_t inpGlobId);

};

#endif