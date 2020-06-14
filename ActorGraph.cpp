#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include <stdlib.h>

#ifndef ASSERT(ec)
#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)
#endif

#include <string>
#include <vector>

class ActorGraph
{
public:
	vector<Actor* > actorList;
	ActorGraph(){}
	void addActor(Actor* newActor);
	void syncActors();
}

void ActorGraph::addActor(Actor* newActor)
{
	actorList.push_back(newActor);
}

void AvtorGraph::syncActors()
{
	
}