#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "connection-type-util.hpp"
#include "Actor.hpp"
#include "ActorGraph.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include "Channel.hpp"
#include "LocalChannel.hpp"
#include "RemoteChannel.hpp"
#include <stdlib.h>
#include <inttypes.h>
#include <algorithm>
#include <cstdlib>
#include <chrono>

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
	int actorElemSize = sizeof(uint64_t);
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
	uint64_t *local_array = (uint64_t*)(gasptr_local_array);

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
                         , i, segment_id_loc_size, 0
                         , sizeof (int), queue_id_size, GASPI_BLOCK
                         )
             );
      	//maxSize = maxSize > *remSize? maxSize: *remSize;
      	//remoteNoActors[i] = *remSize;
      	//gaspi_printf("Read size: %d\n", *remSize);
    }


	ASSERT (gaspi_wait (queue_id_size, GASPI_BLOCK));

	int segSize = 0;
	for(int i = 0; i < num; i++)
	{
		if(i == rank)
			continue;
		segSize += remoteNoActors[i] * actorElemSize;
	}

	gaspi_size_t segment_size_rem_arr = segSize;
	//create segment for receiving actors
	const gaspi_segment_id_t segment_id_rem_array = 3;
	ASSERT (gaspi_segment_create(segment_id_rem_array, segSize
                           , GASPI_GROUP_ALL, GASPI_BLOCK
                           , GASPI_ALLOC_DEFAULT
                           )
     );

	gaspi_pointer_t gasptr_remote_array;
	ASSERT (gaspi_segment_ptr (segment_id_rem_array, &gasptr_remote_array));
	uint64_t* remote_array = (uint64_t*)(gasptr_remote_array);

	int localOffset = 0;
	//read in segment
	for(int i = 0; i < num; i++)
	{
		if(i == rank)
			continue;

		const gaspi_size_t segment_size_cur_rem_arr = actorElemSize * remoteNoActors[i];
		const gaspi_offset_t loc_segment_offset = localOffset;

		gpi_util::wait_if_queue_full (queue_id_data, 1);
		ASSERT (gaspi_read ( segment_id_rem_array, loc_segment_offset
	                     , i, segment_id_loc_array, 0
	                     , segment_size_cur_rem_arr, queue_id_data, GASPI_BLOCK
	                     )
	         );
		localOffset += actorElemSize * remoteNoActors[i];

		
	}
	ASSERT (gaspi_wait (queue_id_data, GASPI_BLOCK));

	//use segmentPointer and push back actors
	for(int j = 0; j < (segSize/actorElemSize); j++)
	{
		remoteActorIDList.push_back(remote_array[j]);
	}

	ASSERT (gaspi_segment_delete(segment_id_loc_size) );
	ASSERT (gaspi_segment_delete(segment_id_rem_size) );
	ASSERT (gaspi_segment_delete(segment_id_loc_array ) );
	ASSERT (gaspi_segment_delete(segment_id_rem_array ) );

	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));
}

void ActorGraph::printActors()
{
	for(int i = 0; i <localActorRefList.size(); i++)
	{
		gaspi_printf("Local actor name %s of %" PRIu64 ", ID %" PRIu64 "\n", (*localActorRefList[i]).name.c_str(), localActorRefList[i]->rank, localActorRefList[i]->globID);
	}
	gaspi_printf("No of actors received: %d\n", remoteActorIDList.size());
	for(int i = 0; i <remoteActorIDList.size(); i++)
	{
		std::pair<int, int> temp = Actor::decodeGlobID(remoteActorIDList[i]);

		gaspi_printf("Non local actor ID %" PRIu64 " no %" PRIu64 " of rank %" PRIu64 " \n", remoteActorIDList[i],temp.first, temp.second);
	}
}
Actor* ActorGraph::getLocalActor(uint64_t globID)
{
	for(int i = 0; i < localActorRefList.size(); i++)
	{
		if(localActorRefList[i]->globID == globID)
			return localActorRefList[i];
	}
	return (new Actor("Not found",0,0));
}
Actor* ActorGraph::getLocalActor(std::string actName)
{
	for(int i = 0; i < localActorRefList.size(); i++)
	{
		if(localActorRefList[i]->name == actName)
			return localActorRefList[i];
	}
	return (new Actor("Not found",0,0));
}
bool ActorGraph::isLocalActor(uint64_t globID)
{
	if(getLocalActor(globID)->name == "Not found")
		return false;
	return true;
}
bool ActorGraph::isLocalActor(std::string actName)
{
	if(getLocalActor(actName)->name == "Not found")
		return false;
	return true;
}
bool ActorGraph::isRemoteActor(uint64_t globID)
{
	for(int i = 0; i < remoteActorIDList.size(); i++)
	{
		if(globID == remoteActorIDList[i])
			return true;
	}
	return false;
}
bool ActorGraph::isRegisteredActor(uint64_t globID)
{
	return (isLocalActor(globID) || isRemoteActor(globID));
}

ActorConnectionType ActorGraph::getActorConnectionType(uint64_t globIDSrcActor, uint64_t globIDDestActor)
{
	if(!isRegisteredActor(globIDSrcActor) || !isRegisteredActor(globIDDestActor))
		return ActorConnectionType::ACTOR_DNE;
	
	bool srcLoc = isLocalActor(globIDSrcActor);
	bool destLoc = isLocalActor(globIDDestActor);
	if(srcLoc && destLoc)
		return ActorConnectionType::LOCAL_LOCAL;
	else if(srcLoc && !destLoc)
		return ActorConnectionType::LOCAL_REMOTE;
	else if(!srcLoc && destLoc)
		return ActorConnectionType::REMOTE_LOCAL;
	else
		return ActorConnectionType::REMOTE_REMOTE;
}

ActorConnectionType ActorGraph::getActorConnectionType(std::pair<uint64_t, uint64_t> curPair)
{
	return getActorConnectionType(curPair.first, curPair.second);
}

void ActorGraph::pushConnection(uint64_t srcGlobID, uint64_t destGlobID)
{
	connectionList.push_back(std::make_pair(srcGlobID, destGlobID));
}

void ActorGraph::makeConnections()
{
	
	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));
	std::sort(connectionList.begin(), connectionList.end());

	std::vector<ActorConnectionType> connectionTypeList;

	for(int i = 0; i < connectionList.size(); i++)
	{
		connectionTypeList.push_back(getActorConnectionType(connectionList[i]));
	}
	//count no of remote connections
	//communicate requirements
	//distribute among queue_max no of queues

	gaspi_number_t maxSeg;
	ASSERT( gaspi_segment_max(&maxSeg));
	int maximumSeg = (int) maxSeg;

	for(int i = 0; i < connectionList.size(); i++)
	{
		switch(connectionTypeList[i])
		{
			case ActorConnectionType::LOCAL_LOCAL:
			{
				//get actors
				Actor* ac1 = getLocalActor(connectionList[i].first);
				Actor* ac2 = getLocalActor(connectionList[i].second);
				//establish channel
				Channel* channel = new LocalChannel();
				//make ports
				InPort* inPort = new InPort(channel);
				OutPort* outPort = new OutPort(channel);

				ac1->addOutPort(outPort);
				ac2->addInPort(inPort);
				
				break;
			}
			case ActorConnectionType::LOCAL_REMOTE:
			{
				//get actors
				Actor* ac1 = getLocalActor(connectionList[i].first);
				//establish channel
				Channel* channel = new RemoteChannel(connectionTypeList[i], i % maximumSeg, 
													connectionList[i].first, connectionList[i].second,
													Actor::decodeGlobID(connectionList[i].second).second);
				//make ports
				OutPort* outPort = new OutPort(channel);

				ac1->addOutPort(outPort);

				break;
			}
			case ActorConnectionType::REMOTE_LOCAL:
			{
				//get actors
				Actor* ac2 = getLocalActor(connectionList[i].second);
				//establish channel
				Channel* channel = new RemoteChannel(connectionTypeList[i], i % maximumSeg, 
													connectionList[i].first, connectionList[i].second,
													Actor::decodeGlobID(connectionList[i].first).second);
				//make ports
				InPort* inPort = new InPort(channel);

				ac2->addInPort(inPort);

				break;
			}
			case ActorConnectionType::REMOTE_REMOTE:
				//do nothing
				break;
		}
	}
	
	ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));
}
double ActorGraph::run()
{	
	//ASSERT (gaspi_barrier (GASPI_GROUP_ALL, GASPI_BLOCK));
	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < localActorRefList.size(); i++)
	{
		localActorRefList[i]->act();
	}

	auto end = std::chrono::steady_clock::now();
	double runTime = std::chrono::duration<double, std::ratio<1>>(end - start).count();
	return runTime;
}