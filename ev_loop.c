#include "ev_loop.h"

#include <stdlib.h>
#include <stdio.h>

/*------- event loop ops--------------------------*/
ev_loop* ev_loop_create()
{
	ev_loop* el;
	ev_handler_table* evht = (ev_handler_table*)malloc(sizeof(ev_handler_table));
	//evht->handlers = (void*)malloc(1024*sizeof(void*));
	el = (ev_loop*)malloc(sizeof(ev_loop));
	el->now = 1;
	el->evlist = ev_list_create();
	el->evht = evht;
	return el;
}

void ev_loop_handle_event(ev_loop* el, ev* e)
{
	void (*handler)(ev_loop*, ev*);	//mmmm: remember this form of decl.
	handler = (void (*)(ev_loop*, ev*))(el->evht->handlers[e->type]);
	e->processed = 1;
	(*handler)(el, e);
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

	//mmmm:temp
	for(i=0;i<el->evlist->size;i++)	{
		e = ev_list_get(el->evlist, i);
		//printf("time=%ld\n", e->time);
	}


	int evlistsize = -1;	//mmm: can its addr be used(e.g. &evlistsize) when not initialized
	while(1)	{
		//printf("evloop 1, now=%ld, evlist.size=%d\n", el->now, el->evlist->size);
		//mmm: if all client ends and server no events, end
		//e = ev_list_pop_head(el->evlist);
		if(el->evlist->size%1000==0)	{
			printf("looping... events queue length: %d\n", el->evlist->size);
		}
		e = ev_list_get(el->evlist, 0);
		//printf("evloop 1.1, etime=%ld\n", e->time);

		if(e==NULL)	{
			break;
		}
		if(e->time>el->now)	{	//if e->time later
			el->now = el->now+1;
			continue;
		}

		ev_list_pop_head(el->evlist);


		e2 = e;

		//printf("evloop 1.5\n");
		ielisttmp = 0;
		while(e2!=NULL && e2->time == e->time)	{
			ev_list_pop_head(el->evlist);
			//printf("while %d, evtype=%d\n", ielisttmp, e2->type);
			elisttmp[ielisttmp++] = e2;
			//e2 = e2->next;
			e2 = ev_list_get(el->evlist, 0);
		}

		//printf("evloop 2\n");
		//i) process ET_CHECK_HB
		for(i=0;i<ielisttmp;i++)	{
			if(elisttmp[i]->type == ET_SERVER_CHECK_HB)	{
				ev_loop_handle_event(el, elisttmp[i]);
				break;
			}
		}

		//ii) processed others
		for(i=0;i<ielisttmp;i++)	{
			if(elisttmp[i]->processed == 0)	{
				ev_loop_handle_event(el, elisttmp[i]);
			}
		}


		//printf("evloop 3, etype=%d\n", e->type);

		//ev_list_gets_by_time(el->evlist, el->now, el->now+demul_interval, evlist, &evlistsize);
		handler = (void (*)(ev_loop*, ev*))(el->evht->handlers[e->type]);
		//printf("handler=%d\n", handler);
		//printf("evloop 4, handler=%d, e->type=%d\n", handler, e->type);
		(*handler)(el, e);

		//printf("evloop end\n");
	}
}

int ev_loop_fire_event(ev_loop* el, ev* e)
{
	//log
	ev_list_add(el->evlist, e);
}
