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
	std::vector<Actor* > localActorRefList;
	std::vector<uint64_t> localActorIDList;
	std::vector<uint64_t> remoteActorIDList;

	std::vector<std::pair<uint64_t, uint64_t> > connectionList;
	
	Actor* getLocalActor(uint64_t globID);
	Actor* getLocalActor(std::string actName);

	bool isLocalActor(uint64_t globID);
	bool isLocalActor(std::string actName);
	bool isRemoteActor(uint64_t globID);
	bool isRegisteredActor(uint64_t globID);


	ActorConnectionType getActorConnectionType(uint64_t globIDSrcActor, uint64_t globIDDestActor);
	ActorConnectionType getActorConnectionType(std::pair<uint64_t, uint64_t> curPair);
	void pushConnection(uint64_t srcGlobID, uint64_t destGlobID);
	void makeConnections();

	double run();

	gaspi_rank_t rank, num;
	ActorGraph();
	void addActor(Actor* newActor);
	void syncActors();
	void printActors();
};