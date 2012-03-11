#include "ev.h"

#include <stdio.h>
#include <stdlib.h>

/*------- event ops--------------------------*/
ev* ev_create(int type, long time)
{
	ev* e = (ev*)malloc(sizeof(ev));
	e->type = type;
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

int ev_list_remove(ev_list* l, int i)
{
	ev* tmp;
	int size;
	ev* e;	//to remove
	ev* pre;
	int j;

	if(!l->head)	{	//head is null
		if(i==0)	{
			return 0;
		}
		else	{
			return -1;
		}
	}

	size = 0;
	tmp = l->head;
	while(tmp!=NULL)	{
		tmp = tmp->next;
		size++;
	}
	if(i>=size)	{
		return -2;
	}

	//init pointer
	pre = NULL;
	e = l->head;

	for(j=0;j<i;j++)	{
		pre = e;
		e = e->next;
	}

	//mmm: right?
	if(pre==NULL)	{	//remove head
		l->head = e->next;
	}
	else	{
		pre->next = e->next;
	}

	ev_destroy(e);

	return 0;
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
		return 0;
	}

	pre = e = l->head;
	while(!e)	{
		if(e->time > enew->time)	{
			break;
		}

		pre = e;
		e = e->next;
	}

	if(e==l->head)	{//insert before head
		enew->next = e;
		l->head = enew;
	}
	else	{	//other
		pre->next = enew;
	}

	return 0;
}

