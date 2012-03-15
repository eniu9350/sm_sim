#include "ev.h"

#include <stdio.h>
#include <stdlib.h>
#include "sm_client.h"

/* ----- */
alisttpl_struct_impl(evdata_server_hb_req)

/*------- event ops--------------------------*/
ev* ev_create(int type, long time)
{
	ev* e = (ev*)malloc(sizeof(ev));
	e->type = type;
	e->time = time;
	e->next = NULL;
	return e;
}

void ev_destroy(ev* e)
{
	free(e->data);
}



/*------- event list ops--------------------------*/
ev_list* ev_list_create()
{
	ev_list* l = (ev_list*)malloc(sizeof(ev_list));
	l->head = NULL;
	l->size = 0;
	return l;
}

ev* ev_list_get(ev_list* l, int i)
{
	int j;
	ev* e;
	e = l->head;

	if(!e)	{
		return NULL;
	}

	for(j=0;j<i;j++)	{
		e = e->next;
		j++;
	}

	return e;
}

ev* ev_list_pop_head(ev_list* l)
{
	ev* e;
	int result;
	e = ev_list_get(l, 0);
	if(e)	{
		//printf("0 get, e not null, to remove\n");
		ev_list_remove_without_destroy(l, 0, &result);	//mmm: no error handling
	}
	return e;
}

void ev_list_gets_by_time(ev_list* l, long start, long end, ev** evlist, int* size)
{
	ev* e;
	int i;
	ev* estart;	//not used
	ev* eend;	//not used
	e = ev_list_get(l, 0);
	while(1)	{
		if(e!=NULL && e->time < start)	{
			e = e->next;
		}
	}

	if(e==NULL)	{
		*size = 0;
		return;
	}

	eend = estart = e;
	*size = 0;

	while(1)	{
		if(e!=NULL && e->time < end)	{
			evlist[*size] = e;
			e = e->next;
			*size = *size + 1;
		}
	}

	eend = e;
}

static ev* ev_list_remove_without_destroy(ev_list* l, int i, int* result)
{
	ev* tmp;
	int size;
	ev* e;	//to remove
	ev* pre;
	int j;

	//printf("111\n");
	if(!l->head)	{	//head is null
		if(i==0)	{
			*result = 0;
			return NULL;
		}
		else	{
			*result = -1;
			return NULL;
		}
	}

	//printf("222\n");
	size = 0;
	tmp = l->head;
	while(tmp!=NULL)	{	//mmm: could get size directly
		tmp = tmp->next;
		size++;
	}
	printf("2.5\n");
	if(i>=size)	{
		*result = -2;
		return NULL;
	}

	//printf("333\n");
	//init pointer
	pre = NULL;
	e = l->head;

	for(j=0;j<i;j++)	{
		pre = e;
		e = e->next;
	}

	//mmm: right?
	*result = 0;
	l->size = l->size-1;
	if(pre==NULL)	{	//remove head
		l->head = e->next;
		return e;	//mmm: right?
	}
	else	{
		pre->next = e->next;
		return e;
	}

}


int ev_list_remove(ev_list* l, int i)
{
	int result;
	ev* e;
	result = 0;
	e = ev_list_remove_without_destroy(l, i, &result);//mmm: [Clang] result used directly?
	if(result == 0)	{
		ev_destroy(e);
	}
	return result;
}

int ev_list_add(ev_list* l, ev* enew)
{
	ev* e;	//insert before e
	ev* pre;	//insert after pre

	if(!l)	{
		return -1;
	}

	if(!l->head)	{
		l->head = enew;
		//printf("enew time=%d\n", enew->time);
		enew->next = NULL;
		l->size = l->size + 1;
		return 0;
	}

	pre = e = l->head;
	while(e)	{
		if(e->time > enew->time)	{
			break;
		}

		pre = e;
		e = e->next;
	}

	enew->next = e;
	if(e==l->head)	{//insert before head
		//printf("e=head%d\n", e);
		l->head = enew;
	}
	else	{	//other
		printf("e!=head\n");
		pre->next = enew;
	}


	l->size = l->size + 1;
	return 0;
}

