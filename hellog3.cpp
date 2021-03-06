#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include <stdlib.h>
#include <stdio.h>

#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)

int main(int argc, char *argv[])
{
	gaspi_rank_t rank, num;
	gaspi_return_t ret;

	ASSERT( gaspi_proc_init(GASPI_BLOCK) );

	ASSERT( gaspi_proc_rank(&rank));
	ASSERT( gaspi_proc_num(&num) );
	gaspi_printf("Hello from ran %d of %d\n", rank, num);
	if(rank == 1)
	{
		gaspi_number_t maxSeg;
		ASSERT( gaspi_segment_max(&maxSeg));
		gaspi_printf("Max segs: %d\n", maxSeg);
		gaspi_number_t quemax, queszmax;
		ASSERT( gaspi_queue_max(&quemax) );
		ASSERT( gaspi_queue_size_max(&queszmax) );
		gaspi_printf("Max q: %d Max q size: %d\n",quemax,queszmax);
	}


	ASSERT( gaspi_proc_term(GASPI_BLOCK) );

	return EXIT_SUCCESS;
}
