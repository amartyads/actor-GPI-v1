#include "Actor.hpp"
#include "connection-type-util.hpp"
#include <stdlib.h>

#pragma once

#include <string>
#include <vector>

class ActorGraph
{
public:
	std::vector<Actor* > localActorRefList;
	std::vector<int> localActorIDList;
	std::vector<int> remoteActorIDList;

	Actor* getLocalActor(int globID);
	Actor* getLocalActor(std::string actName);

	bool isLocalActor(int globID);
	bool isLocalActor(std::string actName);
	bool isRemoteActor(int globID);
	bool isRegisteredActor(int globID);

	ActorConnectionType getActorConnectionType(int globIDSrcActor, int globIDDestActor);

	gaspi_rank_t rank, num;
	ActorGraph();
	void addActor(Actor* newActor);
	void syncActors();
	void printActors();
};