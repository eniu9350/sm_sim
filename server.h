#ifndef _SM_SIM_SERVER_
#define _SM_SIM_SERVER_

#include "event.h"

typedef struct hbreq_list	{
	evdata_hbreq** list;
	int size;
	int capacity;
}hbreq_list;


typedef struct server	{
	hbreq_list* hbreq_buf;
}server;


#endif
