#include "sim_env.h"


#include "input_generator.h"
#include <math.h>
#include <stdlib.h>
#include "ev.h"

sim_env se;

int main1()
{
	ev* e1;
	ev* e2;
	ev* e3;
	ev* e;
	int i;
	e1 = ev_create(3, 100);
	e2 = ev_create(3, 200);
	e3 = ev_create(3, 300);
	ev_list* evlist;
	evlist = ev_list_create();
	ev_list_add(evlist, e1);
	ev_list_add(evlist, e2);
	ev_list_add(evlist, e3);
	e = evlist->head;
	for(i=0;i<3;i++)	{
		printf("%ld\n", e->time);
		e = e->next;
	}
}

int main()
{
	sim_env_init(&se);

	ev_loop_loop(se.el);

	printf("===sum===\nSERVER_SRV_REQ COUNT = %d\n", se.nsrvreq);


	/*
	int n;

	int* d;
	d = (int*)malloc(2500*sizeof(int));
	generate_duration(2500, d);
	*/


}
