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

	Actor *localActor1 = new Actor(rank,0);
	ag.addActor(localActor1);
	Actor *localActor2 = new Actor(rank,1);
	ag.addActor(localActor2);
	//Actor *localActor3 = new Actor(2,rank);
	//ag.addActor(localActor3);


	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));

	ag.syncActors();
	ag.printActors();
	if (rank == 0)
	{
		int g1 = Actor::encodeGlobID(0,0);
		int g2 = Actor::encodeGlobID(0,1);
		int g3 = Actor::encodeGlobID(1,0);
		int g4 = Actor::encodeGlobID(1,1);


		int n1 = static_cast<int>ag.getActorConnectionType(g1,g2);
		int n2 = static_cast<int>ag.getActorConnectionType(g1,g3);
		int n3 = static_cast<int>ag.getActorConnectionType(g3,g1);
		int n4 = static_cast<int>ag.getActorConnectionType(g3,g4);

		gaspi_printf("Connection type %d - %d : %d \n", g1,g2,n1);
		gaspi_printf("Connection type %d - %d : %d \n", g1,g3,n2);
		gaspi_printf("Connection type %d - %d : %d \n", g3,g1,n3);
		gaspi_printf("Connection type %d - %d : %d \n", g3,g4,n4);
	}
	

	ASSERT( gaspi_proc_term(GASPI_BLOCK) );

	return EXIT_SUCCESS;
}