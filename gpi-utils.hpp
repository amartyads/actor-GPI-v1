#ifndef ACTORGPI_UTILS_HPP
#define ACTORGPI_UTILS_HPP


#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include <stdlib.h>
#include <stdexcept>

namespace gpi_util
{
	static void success_or_exit(const char* file, const int line, const int ec)
	{
		if(ec!=GASPI_SUCCESS)
		{
			gaspi_printf("Assertions failed in $s[%i]:%d\n",file,line,ec);
			exit(EXIT_FAILURE);
		}
	}
	static void wait_if_queue_full ( const gaspi_queue_id_t queue_id
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
	static int test_notif_or_exit ( gaspi_segment_id_t segment_id,
									gaspi_notification_id_t notification_id,
									gaspi_notification_t expected)
	{
		gaspi_notification_id_t id;
		gaspi_return_t ret;
		if((ret = gaspi_notify_waitsome(segment_id, notification_id, 1, &id, GASPI_TEST)) == GASPI_SUCCESS)
		{
			if(id != notification_id)
				throw std::runtime_error("ID not equal to notification ID");
			
			gaspi_notification_t value;
			success_or_exit(__FILE__, __LINE__, gaspi_notify_reset(segment_id, id, &value));
			if(value != expected)
				throw std::runtime_error("Notification unexpected value");

			return 1;
		}
		else
		{
			if(ret == GASPI_ERROR)
				throw std::runtime_error("Error while waiting for notification");
			
			return 0;
		}
		
	}

}



#endif
