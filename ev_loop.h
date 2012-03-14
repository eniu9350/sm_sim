#ifndef _SM_SIM_EVENTLOOP_
#define _SM_SIM_EVENTLOOP_

#include "ev.h"

/*------- event loop --------------------------*/
typedef struct ev_loop	{
	long now;
	//evhandler_table* ht;
	ev_list* evlist;
	ev_handler_table* evht;
}ev_loop;


/*------- event loop ops--------------------------*/
ev_loop* ev_loop_create();
void ev_loop_loop(ev_loop* el);
//wrapper of ev_list_add, incorporating logging support
int ev_loop_fire_event(ev_loop* el, ev* e);

#endif
