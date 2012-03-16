#include "sim_env.h"

#include "ev_loop.h"
#include "ev.h"
#include <stdlib.h>
#include "input_generator.h"
#include <stdio.h>
#include "ev_handler.h"

extern sim_env se;

void sim_env_init(sim_env* se)
{
	int i, j;
	long ltemp;
	int itemp;
	ev* e;
	sm_client* clients;
	sm_client* client;
	evdata_client_switching* ed_cs;
	evdata_client_hb_req* ed_chr;	
	evdata_server_bc_req* ed_sbr;
	evdata_client_srv_req* ed_csr;


	/* --- basic init --- */
	se->el = ev_loop_create();
	se->el->evht->handlers[ET_SERVER_HB_REQ] = ev_handle_server_hb_req;
	se->el->evht->handlers[ET_SERVER_HB_RSP] = ev_handle_server_hb_resp;
	se->el->evht->handlers[ET_SERVER_BC_REQ] = ev_handle_server_bc_req;
	se->el->evht->handlers[ET_SERVER_CHECK_HB] = ev_handle_server_check_hb;
	se->el->evht->handlers[ET_SERVER_SRV_REQ] = ev_handle_server_srv_req;


	se->el->evht->handlers[ET_CLIENT_POWER_ON] = ev_handle_client_power_on;
	se->el->evht->handlers[ET_CLIENT_HB_REQ] = ev_handle_client_hb_req;
	se->el->evht->handlers[ET_CLIENT_BC_REQ] = ev_handle_client_bc_req;
	se->el->evht->handlers[ET_CLIENT_SWITCHING] = ev_handle_client_switching;
	se->el->evht->handlers[ET_CLIENT_SRV_REQ] = ev_handle_client_srv_req;


	se->nclients = CLIENT_COUNT;
	//mmm: start end of server not set
	se->server = sm_server_create();

	se->nsrvreq = 0;
	

	/*--- add clients ---*/
	clients = generate_input_clients_simple_1(se->nclients, CHANNEL_COUNT);

	se->clients = (sm_client**)malloc(se->nclients*sizeof(sm_client*));
	for(i=0;i<se->nclients;i++)	{
		se->clients[i] = clients+i;
	}

	printf("simenvinit 1\n");

	//mmm: not implemented yet!
	/*-- add client poweron event--*/

	/*-- add client channel change event--*/
	for(i=0;i<se->nclients;i++)	{
	printf("simenvinit 1.5, i=%d\n", i);
		client = se->clients[i];
		ltemp = client->plan->arrival;
		for(j=0;j<client->plan->nswitchings;j++)	{
			if(j==0)	{
				//mmm: not implemented
				//not switching
				e = ev_create(ET_CLIENT_SRV_REQ, ltemp);
				ed_csr = (evdata_client_srv_req*)malloc(sizeof(evdata_client_srv_req));
				ed_csr->uid = client->id;
				ed_csr->chid = client->plan->switchings[j]->chid;
				e->data = ed_csr;
				e->agent = (void*)client;
			}
			else	{
				e = ev_create(ET_CLIENT_SWITCHING, ltemp);
				ed_cs = (evdata_client_switching*)malloc(sizeof(evdata_client_switching));
				ed_cs->chid = client->plan->switchings[j]->chid;
				e->data = (void*)ed_cs;
				e->agent = (void*)client;
			}
			printf("ltemp=%ld\n", ltemp);
			ltemp += client->plan->switchings[j]->duration;

			ev_list_add(se->el->evlist, e);
			printf("evlistsize=%d, added e time=%ld, lasttime=%ld\n", se->el->evlist->size, e->time, (ev_list_get(se->el->evlist, se->el->evlist->size-1))->time );
		}
	}

	//mmmm:temp
	for(i=0;i<se->el->evlist->size;i++)	{
		e = ev_list_get(se->el->evlist, i);
		printf("in sim1, time=%ld\n", e->time);
	}

	printf("simenvinit 2\n");
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

	printf("simenvinit 3\n");
	/*-- add server broadcast event--*/
	ltemp = 30;	//mmm: bc interval, should be loaded
	for(i=se->server->start+ltemp;i<se->server->end;i+=ltemp)	{	//mmm: < should be <=?
		printf("simenvinit 3.1 i=%d\n", i);
		e = ev_create(ET_SERVER_BC_REQ, i);
		ed_sbr = (evdata_server_bc_req*)malloc(sizeof(evdata_server_bc_req));
		e->data = (void*)ed_sbr;
		e->agent = (void*)se->server;

		ev_list_add(se->el->evlist, e);
	}
	printf("simenvinit end, server.cisize=%d\n", se->server->ci->size);

	//mmmm:temp
	for(i=0;i<se->el->evlist->size;i++)	{
		e = ev_list_get(se->el->evlist, i);
		printf("in sim, time=%ld\n", e->time);
	}
}


/* ------ client -----------*/
sm_client* sim_env_get_client_by_uid(int uid)
{
	sm_client* c;
	int i;
	for(i=0;i<se.nclients;i++)	{
		c = se.clients[i];
		if(c->id == uid)	{
			return c;
		}
	}

	return NULL;
}

