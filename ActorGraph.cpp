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
	localActorRefList.push_back(newActor);
	localActorIDList.push_back(newActor->globID);
}

void ActorGraph::syncActors()
{
	int actorElemSize = sizeof(int);
	//int remoteNoActors[num];

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
 	ASSERT (gaspi_segment_create(segment_id_rem_size, (num * sizeof(int))
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
	int *remoteNoActors = (int *)(dstgasp);

	*locSize = localActorRefList.size();
	//int maxSize = *locSize;

	//segment for local actors
	const gaspi_segment_id_t segment_id_loc_array = 2;

	const gaspi_size_t segment_size_arr = actorElemSize * (*locSize);

	ASSERT (gaspi_segment_create(segment_id_loc_array, segment_size_arr
                               , GASPI_GROUP_ALL, GASPI_BLOCK
                               , GASPI_ALLOC_DEFAULT
                               )
         );

	gaspi_pointer_t gasptr_local_array;
	ASSERT (gaspi_segment_ptr (segment_id_loc_array, &gasptr_local_array));
	int *local_array = (int*)(gasptr_local_array);

	for(int i = 0; i < *locSize; i++)
		local_array[i] = localActorIDList[i];

	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));

	//for each other rank
	for(int i = 0; i < num; i++)
	{
		if(i == rank)
			continue;
		//read no of actors
		gpi_util::wait_if_queue_full (queue_id_size, 1);
 		
 		const gaspi_offset_t offset_dst = i * sizeof (int);
      	ASSERT (gaspi_read ( segment_id_rem_size, offset_dst
                         , rank, segment_id_loc_size, 0
                         , sizeof (int), queue_id_size, GASPI_BLOCK
                         )
             );
      	//maxSize = maxSize > *remSize? maxSize: *remSize;
      	//remoteNoActors[i] = *remSize;
      	//gaspi_printf("Read size: %d\n", *remSize);
    }



	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));

	//create pointer for receiving actors
	gaspi_size_t segment_size_rem_arr = actorElemSize * maxSize;
	//create segment for receiving actors
	const gaspi_segment_id_t segment_id_rem_array = 3;
	ASSERT (gaspi_segment_create(segment_id_rem_array, segment_size_rem_arr
                           , GASPI_GROUP_ALL, GASPI_BLOCK
                           , GASPI_ALLOC_DEFAULT
                           )
     );

	gaspi_pointer_t gasptr_remote_array;
	ASSERT (gaspi_segment_ptr (segment_id_rem_array, &gasptr_remote_array));
	int* remote_array = (int*)(gasptr_remote_array);


	//read in segment
	for(int i = 0; i < num; i++)
	{
		if(i == rank)
			continue;

		const gaspi_size_t segment_size_cur_rem_arr = actorElemSize * remoteNoActors[i];

		gpi_util::wait_if_queue_full (queue_id_data, 1);
		ASSERT (gaspi_read ( segment_id_rem_array, 0
	                     , rank, segment_id_loc_array, 0
	                     , segment_size_cur_rem_arr, queue_id_data, GASPI_BLOCK
	                     )
	         );
		//use segmentPointer and push back actors
		for(int j = 0; j < remoteNoActors[i]; j++)
		{
			nonLocalActorIDList.push_back(remote_array[i]);
		}
	}

	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));

}

void ActorGraph::printActors()
{
	for(int i = 0; i <localActorRefList.size(); i++)
	{
		gaspi_printf("Local actor name %s of %d, ID %d\n", (*localActorRefList[i]).name.c_str(), localActorRefList[i]->rank, localActorRefList[i]->globID);
	}
	gaspi_printf("No of actors received: %d\n", nonLocalActorIDList.size());
	for(int i = 0; i <nonLocalActorIDList.size(); i++)
	{
		gaspi_printf("Non local actor reference ID %d\n", nonLocalActorIDList[i]);
	}
}