#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include <stdlib.h>

#ifndef ASSERT
#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)
#endif

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
};

ActorGraph::ActorGraph()
{
	ASSERT( gaspi_proc_rank(&rank));
	ASSERT( gaspi_proc_num(&num) );
}

void ActorGraph::addActor(Actor* newActor)
{
	actorList.push_back(newActor);
}

void ActorGraph::syncActors()
{
	gaspi_printf("Hello from ran %d of %d\n", rank, num);
}