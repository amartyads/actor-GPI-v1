#include "Actor.hpp"
#include "connection-type-util.hpp"
#include <stdlib.h>
#include <utility>

#pragma once

#include <string>
#include <vector>
#include <cstdint>

class ActorGraph
{
public:

	gaspi_rank_t rank, num;
	std::vector<Actor* > localActorRefList;
	std::vector<uint64_t> localActorIDList;
	std::vector<uint64_t> remoteActorIDList;

	std::vector<std::pair<uint64_t, uint64_t> > connectionList;
	std::vector<uint64_t> dataBlocksInSegment;
	std::vector<uint64_t> offsetVals;
	int dataBlockSize, dataQueueLen;
	
	Actor* getLocalActor(uint64_t globID);
	Actor* getLocalActor(std::string actName);

	bool isLocalActor(uint64_t globID);
	bool isLocalActor(std::string actName);
	bool isRemoteActor(uint64_t globID);
	bool isRegisteredActor(uint64_t globID);


	ActorConnectionType getActorConnectionType(uint64_t globIDSrcActor, uint64_t globIDDestActor);
	ActorConnectionType getActorConnectionType(std::pair<uint64_t, uint64_t> curPair);
	void pushConnection(uint64_t srcGlobID, uint64_t destGlobID);
	void sortConnections();
	void makeConnections();

	void genOffsets();
	std::string getOffsetString();

	double run();

	ActorGraph();
	ActorGraph(int dataBlockSize, int dataBlockLen);
	void addActor(Actor* newActor);
	void syncActors();
	void printActors();
};