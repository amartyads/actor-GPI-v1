
#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include <stdlib.h>

#pragma once

namespace gpi_util
{
	void success_or_exit(const char* file, const int line, const int ec)
	{
		if(ec!=GASPI_SUCCESS)
		{
			gaspi_printf("Assertions failed in $s[%i]:%d\n",file,line,ec);
			exit(EXIT_FAILURE);
		}
	}
	void wait_if_queue_full ( const gaspi_queue_id_t queue_id
                        , const gaspi_number_t request_size
                        )
	{
	  gaspi_number_t queue_size_max;
	  gaspi_number_t queue_size;
	 
	  success_or_exit (__FILE__, __LINE__, gaspi_queue_size_max (&queue_size_max));
	  success_or_exit (__FILE__, __LINE__, gaspi_queue_size (queue_id, &queue_size));
	 
	  if (queue_size + request_size >= queue_size_max)
	    {
	      success_or_exit (__FILE__, __LINE__, gaspi_wait (queue_id, GASPI_BLOCK));
	    }
	}

}


