#include "Actor.hpp"
#include "connection-type-util.hpp"
#include <stdlib.h>
#include <utility>

#pragma once

#include <string>
#include <vector>

class ActorGraph
{
public:
	std::vector<Actor* > localActorRefList;
	std::vector<int> localActorIDList;
	std::vector<int> remoteActorIDList;

	std::vector<std::pair<int, int> > connectionList;
	
	Actor* getLocalActor(int globID);
	Actor* getLocalActor(std::string actName);

	bool isLocalActor(int globID);
	bool isLocalActor(std::string actName);
	bool isRemoteActor(int globID);
	bool isRegisteredActor(int globID);


	ActorConnectionType getActorConnectionType(int globIDSrcActor, int globIDDestActor);
	ActorConnectionType getActorConnectionType(std::pair<int, int> *curPair);
	void pushConnection(int srcGlobID, int destGlobID);
	void makeConnections();

	gaspi_rank_t rank, num;
	ActorGraph();
	void addActor(Actor* newActor);
	void syncActors();
	void printActors();
};