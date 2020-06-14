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
	int actorPtrSize = 0;
	if(actorList.empty())
		actorPtrSize = sizeof(new Actor("temp",99));
	else
		actorPtrSize = sizeof(actorList[0]);

	const gaspi_queue_id_t queue_id_size = 0;
	const gaspi_queue_id_t queue_id_data = 1;

	//initialize segment for sending segsize
	//initialize segment for receiving segsize
	const gaspi_segment_id_t segment_id_loc_size = 0;
  	const gaspi_segment_id_t segment_id_rem_size = 1;

  	ASSERT (gaspi_segment_create(segment_id_loc_size, sizeof(int)
                               , GASPI_GROUP_ALL, GASPI_BLOCK
                               , GASPI_ALLOC_DEFAULT
                               )
         );
 	ASSERT (gaspi_segment_create(segment_id_rem_size, sizeof(int)
                               , GASPI_GROUP_ALL, GASPI_BLOCK
                               , GASPI_ALLOC_DEFAULT
                               )
         );

 	gaspi_pointer_t srcgasp, dstgasp;
 
	/* get initial pointers to each segment */
	ASSERT (gaspi_segment_ptr (segment_id_loc_size, &srcgasp));
	ASSERT (gaspi_segment_ptr (segment_id_rem_size, &dstgasp));
	
	const gaspi_queue_id_t queue_id = 0;
	 
	int *locSize = (int *)(srcgasp);
	int *remSize = (int *)(dstgasp);

	*locSize = actorList.size();

	//segment for local actors
	const gaspi_segment_id_t segment_id_loc_array = 2;

	const gaspi_size_t segment_size_arr = sizeof(actorList); // arrayPtrSize * (*locSize)

	ASSERT (gaspi_segment_create(segment_id_loc_array, segment_size_arr
                               , GASPI_GROUP_ALL, GASPI_BLOCK
                               , GASPI_ALLOC_DEFAULT
                               )
         );

	gaspi_pointer_t gasptr_local_array;
	ASSERT (gaspi_segment_ptr (segment_id_loc_array, &gasptr_local_array));
	Actor** local_array = (Actor **)(gasptr_local_array);

	for(int i = 0; i < *locSize; i++)
		local_array[i] = actorList[i];

	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));

	int segmentCtr = 3;
	//for each other rank
	for(int i = 0; i < num, i++)
	{
		if(i == rank)
			continue;
		//read no of actors
		gpi_util::wait_if_queue_full (queue_id, 1);
 
      	ASSERT (gaspi_read ( segment_id_rem_size, 0
                         , rank, segment_id_loc_size, 0
                         , sizeof (int), queue_id_size, GASPI_BLOCK
                         )
             );
		//create pointer for receiving actors
		gaspi_size_t segment_size_rem_arr = arrayPtrSize * (*remSize);
		//create segment for receiving actors
		gaspi_segment_id_t segment_id_rem_array = ctr++;
		ASSERT (gaspi_segment_create(segment_id_rem_array, segment_size_rem_arr
                               , GASPI_GROUP_ALL, GASPI_BLOCK
                               , GASPI_ALLOC_DEFAULT
                               )
         );

		gaspi_pointer_t gasptr_remote_array;
		ASSERT (gaspi_segment_ptr (segment_id_rem_array, &gasptr_remote_array));
		Actor** remote_array = (Actor **)(gasptr_remote_array);
		//read in segment
		ASSERT (gaspi_read ( segment_id_rem_arr, 0
                         , rank, segment_id_loc_array, 0
                         , segment_size_rem_arr, queue_id_data, GASPI_BLOCK
                         )
             );
		//use segmentPointer and push back actors
		for(int j = 0; j < *remSize; j++)
		{
			actorList.push_back(remote_array[i]);
		}
	}
}

void ActorGraph::printActors()
{
	for(int i = 0; i < actorList.size(); i++)
	{
		gaspi_printf("Actor name %s of %d, address %p\n", (*actorList[i]).name.c_str(), actorList[i]->rank, (void *)actorList[i]);
	}
}