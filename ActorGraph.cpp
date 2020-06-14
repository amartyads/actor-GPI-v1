#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include "ActorGraph.hpp"
#include <stdlib.h>

#ifndef ASSERT
#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)
#endif

#include <string>
#include <vector>

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
	;
}

void ActorGraph::printActors()
{
	for(int i = 0; i < actorList.size(); i++)
	{
		gaspi_printf("Actor name %s of %d, address %p\n", (*actorList[i]).name.c_str(), actorList[i]->rank, (void *)actorList[i]);
	}
}