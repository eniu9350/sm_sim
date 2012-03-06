#ifndef _SM_SIM_COMMON_
#define _SM_SIM_COMMON_

#include "event.h"


typedef struct context_global	{
	long now;
	event_loop* el;
}context_global;

#endif
