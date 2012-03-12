#include "sim_env.h"

#include "ev_loop.h"
#include "ev.h"
#include <stdlib.h>

void sim_env_init(sim_env* se)
{
	int i, j;
	long ltemp;
	int itemp;
	ev* e;
	evdata_client_switching* evdata_cs;
	//mmm: not implemented yet!
	/*-- add client poweron event--*/

	/*-- add client channel change event--*/
	for(i=0;i<se->nclients;i++)	{
		ltemp = se->clients[i]->plan->arrival;
		for(j=0;j<se->clients[i]->plan->nswitchings;j++)	{
			if(j==0)	{
				//mmm: not implemented
				//not switching
			}
			else	{
				e = ev_create(ET_CLIENT_SWITCHING, ltemp);
				evdata_cs = (evdata_client_switching*)malloc(sizeof(evdata_client_switching));
				evdata_cs->chid = se->clients[i]->plan->switchings[j]->chid;
				e->data = (void*)evdata_cs;
				e->agent = (void*)se->clients[i];
			}
			ltemp += se->clients[i]->plan->switchings[j]->lasting;

			ev_list_add(se->el->evlist, e);
		}
	}

	/*-- add client heartbeat event--*/
	ltemp = 30;	//hb interval
	

	
}
