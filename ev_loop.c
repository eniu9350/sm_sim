#include "ev_loop.h"

#include <stdlib.h>

/*------- event loop ops--------------------------*/
ev_loop* ev_loop_create()
{
	ev_loop* el;
	ev_handler_table* evht = (ev_handler_table*)malloc(sizeof(ev_handler_table));
	//evht->handlers = (void*)malloc(1024*sizeof(void*));
	el = (ev_loop*)malloc(sizeof(ev_loop));
	el->evlist = ev_list_create();
	el->evht = evht;
}

void ev_loop_loop(ev_loop* el)
{
	ev* e;
	ev* e2;
	ev** elisttmp;
	int ielisttmp;
	long demul_interval;	//mmm
	ev** evlist;
	void (*handler)(ev_loop*, ev*);
	int i;

	evlist = (ev**)malloc(20000*sizeof(ev*));

	elisttmp = (ev**)malloc(20000*sizeof(ev*));
	

	int evlistsize = -1;	//mmm: can its addr be used(e.g. &evlistsize) when not initialized
	while(1)	{
		printf("evloop 1, evlistsize=%d\n", el->evlist->size);
		//mmm: if all client ends and server no events, end
		e = ev_list_get(el->evlist, 0);
		e2 = e;
		
		printf("evloop 1.5\n");
		ielisttmp = 0;
		while(e2->time == e->time)	{
			printf("while %d\n", ielisttmp);
			elisttmp[ielisttmp++] = e2;
			e2 = e2->next;
		}

		printf("evloop 2\n");
		//i) process ET_CHECK_HB
		for(i=0;i<ielisttmp;i++)	{
			if(elisttmp[i]->type == ET_SERVER_CHECK_HB)	{
				break;
			}
		}
		ev_loop_fire_event(el, elisttmp[i]);

			
		printf("evloop 3\n");
		
		//ev_list_gets_by_time(el->evlist, el->now, el->now+demul_interval, evlist, &evlistsize);
		handler = (void (*)(ev_loop*, ev*))(el->evht->handlers[e->type]);
		(*handler)(el, e);

	}
}

int ev_loop_fire_event(ev_loop* el, ev* e)
{
	//log
	ev_list_add(el->evlist, e);
}
