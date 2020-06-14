#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "Actor.hpp"
#include "ActorGraph.hpp"
#include <stdlib.h>

#ifndef ACTORGPI_UTILS_HPP
#include "gpi-utils.hpp"
#endif

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
	gaspi_printf("Hello from ran %d of %d\n", rank, num);
}