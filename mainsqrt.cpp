#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include "SqrtSourceActor.hpp"
#include "SqrtSqrtActor.hpp"
#include "SqrtSinkActor.hpp"
#include "ActorGraph.hpp"
#include <stdlib.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <stdexcept>

#ifndef ASSERT
#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)
#endif

int main(int argc, char *argv[])
{
	gaspi_rank_t rank, num;
	gaspi_return_t ret;

	ASSERT( gaspi_proc_init(GASPI_BLOCK) );

	int maxVals = 2;
	int queueMax = 3;

	ActorGraph ag(maxVals, queueMax);

	ASSERT( gaspi_proc_rank(&rank));
	ASSERT( gaspi_proc_num(&num) );

    if(num != 3)
    {
        throw std::runtime_error("Need 3 ranks");
    }

    int numDigs = 5;
    Actor* localActor;

	switch (rank)
    {
    case 0:
        localActor = new SqrtSourceActor("Source", rank, 0, numDigs);
        break;
    case 1:
        localActor = new SqrtSqrtActor("Rooter", rank, 0, numDigs);
        break;
    case 2:
        localActor = new SqrtSinkActor("Sink", rank, 0, numDigs);
        break;
    
    default:
        localActor = nullptr;
        break;
    }

    if(localActor == nullptr)
    {
        throw std::runtime_error("Actor created null");
    }

    ag.addActor(localActor);

	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));

	ag.syncActors();
	ag.printActors();

	ag.pushConnection(0,Actor::encodeGlobID(1,0));
	ag.pushConnection(Actor::encodeGlobID(1,0),Actor::encodeGlobID(2,0));
	
	ag.makeConnections();


	int i = 0;
	while(! (localActor->finished))
	//while(i < 7)
	{
		gaspi_printf("Run %d from rank %d\n",i++,rank);
		double rt = ag.run();
		gaspi_printf("Runtime from rank %d: %lf\n",rank,rt);
		//if(rank == 0)
			std::cout << std::endl;
		//ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));
	}
	gaspi_printf("Rank %d done.\n",rank);	
	
	gpi_util::wait_for_flush_queues();

	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));
	//ag.sortConnections();
	//ag.genOffsets();
	//std::string offstr = ag.getOffsetString();

	//std::cout << "Rank " <<rank<<" offset string " << offstr << std::endl;

	ASSERT( gaspi_proc_term(GASPI_BLOCK) );

	return EXIT_SUCCESS;
}