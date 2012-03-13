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
	sm_client* client;
	evdata_client_switching* ed_cs;
	evdata_client_hb_req* ed_chr;	
	evdata_server_bc_req* ed_sbr;
	evdata_client_srv_req* ed_csr;
	//mmm: not implemented yet!
	/*-- add client poweron event--*/

	/*-- add client channel change event--*/
	for(i=0;i<se->nclients;i++)	{
		client = se->clients[i];
		ltemp = client->plan->arrival;
		for(j=0;j<client->plan->nswitchings;j++)	{
			if(j==0)	{
				//mmm: not implemented
				//not switching
			}
			else	{
				e = ev_create(ET_CLIENT_SWITCHING, ltemp);
				ed_cs = (evdata_client_switching*)malloc(sizeof(evdata_client_switching));
				ed_cs->chid = client->plan->switchings[j]->chid;
				e->data = (void*)ed_cs;
				e->agent = (void*)client;
			}
			ltemp += client->plan->switchings[j]->duration;

			ev_list_add(se->el->evlist, e);
		}
	}

	/*-- add client heartbeat event--*/
	ltemp = 30;	//mmm: hb interval, should be loaded
	for(i=0;i<se->nclients;i++)	{
		client = se->clients[i];
		for(j=client->plan->arrival+ltemp;j<client->plan->departure;j+=ltemp)	{	//mmm: check whether departure time is right
			e = ev_create(ET_CLIENT_HB_REQ, j);
			ed_chr = (evdata_client_hb_req*)malloc(sizeof(evdata_client_hb_req));
			e->data = (void*)ed_chr;
			e->agent = (void*)client;

			ev_list_add(se->el->evlist, e);
		}
	}

	/*-- add server broadcast event--*/
	ltemp = 30;	//mmm: bc interval, should be loaded
	for(i=se->server->start+ltemp;i<se->server->end;i+=ltemp)	{	//mmm: < should be <=?
		e = ev_create(ET_SERVER_BC_REQ, i);
		ed_sbr = (evdata_server_bc_req*)malloc(sizeof(evdata_server_bc_req));
		e->data = (void*)ed_sbr;
		e->agent = (void*)se->server;

		ev_list_add(se->el->evlist, e);
	}

}
