#ifndef _SM_SIM_COMMON_
#define _SM_SIM_COMMON_

#include "event.h"

struct ev_loop;

typedef struct context_global	{
	long now;
	struct ev_loop* el;
}context_global;

#endif
