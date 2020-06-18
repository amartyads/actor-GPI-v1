#include "Actor.hpp"
#include <stdlib.h>

#pragma once

#include <string>
#include <vector>

class ActorGraph
{
public:
	std::vector<Actor* > localActorRefList;
	std::vector<int> localActorIDList;
	std::vector<int> nonLocalActorIDList;

	Actor* getLocalActor(int globID);
	Actor* getLocalActor(std::string actName);

	gaspi_rank_t rank, num;
	ActorGraph();
	void addActor(Actor* newActor);
	void syncActors();
	void printActors();
};