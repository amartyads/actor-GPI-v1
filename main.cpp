#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include "ActorGraph.hpp"
#include <stdlib.h>

#ifndef ASSERT
#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)
#endif

int main(int argc, char *argv[])
{
	gaspi_rank_t rank, num;
	gaspi_return_t ret;

	ASSERT( gaspi_proc_init(GASPI_BLOCK) );

	ActorGraph ag;

	ASSERT( gaspi_proc_rank(&rank));
	ASSERT( gaspi_proc_num(&num) );

	Actor *localActor1 = new Actor(0,rank);
	ag.addActor(localActor1);
	//Actor *localActor2 = new Actor(1,rank);
	//ag.addActor(localActor2);
	//Actor *localActor3 = new Actor(2,rank);
	//ag.addActor(localActor3);


	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));

	ag.syncActors();
	ag.printActors();

	Actor* temp = ag.getLocalActor(Actor::encodeGlobID(0,0));
	if(temp->name == "Not found")
		gaspi_printf("Actor not found on rank %d\n",rank);
	else
		gaspi_printf("Actor found on rank %d\n",rank);

	ASSERT( gaspi_proc_term(GASPI_BLOCK) );

	return EXIT_SUCCESS;
}