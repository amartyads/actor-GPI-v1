#include "Actor.hpp"
#include <stdlib.h>

#pragma once

#include <string>
#include <vector>

class ActorGraph
{
public:
	std::vector<Actor* > actorList;
	gaspi_rank_t rank, num;
	ActorGraph();
	void addActor(Actor* newActor);
	void syncActors();
	void printActors();
};