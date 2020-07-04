#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include "ActorGraph.hpp"
#include <stdlib.h>
#include <cstdint>

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
	
	double rt = ag.run();
	gaspi_printf("Runtime from rank %d: %lf\n",rank,rt);

	ASSERT( gaspi_proc_term(GASPI_BLOCK) );

	return EXIT_SUCCESS;
}