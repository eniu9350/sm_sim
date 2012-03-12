#ifndef _SM_SIM_SM_SERVER_
#define _SM_SIM_SM_SERVER_

#include "ev_loop.h"

#include "ch.h"

typedef struct sm_server	{
	//ev_loop* el;
//	hbreq_list* hbreq_buf;
	long start;
	long end;
	
	ch_info* ci;

	evdata_server_hb_req_list* buf_hb_req;

}sm_server;


#endif

