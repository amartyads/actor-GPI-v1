#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include "ActorGraph.hpp"
#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include <string>

#ifndef ASSERT
#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)
#endif

int main(int argc, char *argv[])
{
	gaspi_rank_t rank, num;
	gaspi_return_t ret;

	ASSERT( gaspi_proc_init(GASPI_BLOCK) );

	int maxVals = 10;
	int queueMax = 3;

	ActorGraph ag(maxVals, queueMax);

	ASSERT( gaspi_proc_rank(&rank));
	ASSERT( gaspi_proc_num(&num) );

	Actor *localActor1 = new Actor(rank,0);
	Actor *localActor2 = new Actor(rank,1);
	//Actor *localActor3 = new Actor(rank,2);

	//ag.addActor(localActor3);
	ag.addActor(localActor1);
	ag.addActor(localActor2);


	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));

	ag.syncActors();
	ag.printActors();
	
	ag.pushConnection(0,1);
	ag.pushConnection(1,Actor::encodeGlobID(1,0));
	//ag.pushConnection(0,Actor::encodeGlobID(1,0));
	//ag.pushConnection(Actor::encodeGlobID(1,0),Actor::encodeGlobID(2,0));
	//ag.pushConnection(Actor::encodeGlobID(1,0),Actor::encodeGlobID(0,1));
	ag.pushConnection(Actor::encodeGlobID(1,0),Actor::encodeGlobID(1,1));
	//ag.pushConnection(Actor::encodeGlobID(1,1),Actor::encodeGlobID(2,1));
	//ag.pushConnection(Actor::encodeGlobID(1,0),1);
	//ag.pushConnection(1,Actor::encodeGlobID(1,1));
	//ag.pushConnection(Actor::encodeGlobID(1,1),Actor::encodeGlobID(2,0));
	//ag.pushConnection(Actor::encodeGlobID(2,0),Actor::encodeGlobID(2,1));

	//ag.makeConnections();
	/*
	int i = 0;
	while(! (localActor1->receivedData && localActor2->receivedData))// && localActor3->receivedData))
	{
		
		gaspi_printf("Run %d from rank %d\n",i++,rank);
		double rt = ag.run();
		gaspi_printf("Runtime from rank %d: %lf\n",rank,rt);
		//if(rank == 0)
			std::cout << std::endl;
	}
	gaspi_printf("Rank %d done.\n",rank);	
	*/

	ag.sortConnections();
	ag.genOffsets();
	std::string offstr = ag.getOffsetString();

	std::cout << "Rank " <<rank<<" offset string " << offstr << std::endl;

	ASSERT( gaspi_proc_term(GASPI_BLOCK) );

	return EXIT_SUCCESS;
}