#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include <stdlib.h>

#pragma once

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