#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
#include "Actor.hpp"
#include <stdlib.h>

#ifndef ASSERT(ec)
#define ASSERT(ec) gpi_util::success_or_exit(__FILE__,__LINE__,ec)
#endif

int main(int argc, char *argv[])
{
	gaspi_rank_t rank, num;
	gaspi_return_t ret;

	ASSERT( gaspi_proc_init(GASPI_BLOCK) );

	ASSERT( gaspi_proc_rank(&rank));
	ASSERT( gaspi_proc_num(&num) );
	gaspi_printf("Hello from ran %d of %d\n", rank, num);

	ASSERT( gaspi_proc_term(GASPI_BLOCK) );

	return EXIT_SUCCESS;
}