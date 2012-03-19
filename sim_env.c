#include "sim_env.h"

#include "ev_loop.h"
#include "ev.h"
#include <stdlib.h>
#include "input_generator.h"
#include <stdio.h>
#include "ev_handler.h"
#include <string.h>

extern sim_env se;


static void sim_env_config_set_types(value_type* types, int n)
{
	int i;
	for(i=0;i<n;i++)	{
		se.config->types[i].id = types[i].id;
		se.config->types[i].type = types[i].type;
	}
}

static void sim_env_config_init()
{
	se.config = (sim_config*)malloc(sizeof(sim_config));
}

static void sim_env_config_load()
{
	value_type types[2] = {{VALUE_ID_INTERVAL_CLIENT_HEARTBEAT, VALUE_TYPE_LONG}, {VALUE_ID_INTERVAL_SERVER_BROADCAST, VALUE_TYPE_LONG}};
	sim_env_config_set_types(types,2);
}

void sim_env_init()
{
	int i, j;
	long ltemp;
	long end;
	int itemp;
	ev* e;
	sm_client* clients;
	sm_client* client;
	evdata_client_switching* ed_cs;
	evdata_client_hb_req* ed_chr;	
	evdata_server_bc_req* ed_sbr;
	evdata_client_srv_req* ed_csr;


	/*--- show parameter settings ---*/
	printf("************ Parameters ************\n");
	printf("CH=%d, CLIENT=%d, SWITCHINGS(per client)=%d\n", CHANNEL_COUNT, CLIENT_COUNT, SWITCHING_COUNT);
	printf("HB_INTERVAL=%d, BC_INTERVAL=%d\n", HEARTBEAT_INTERVAL, BROADCAST_INTERVAL);
	printf("\n");

	/*--- config init ---*/
	sim_env_config_init();
	sim_env_config_load();

	sim_env_stat_init();
	


	/* --- basic init --- */
	se.el = ev_loop_create();
	se.el->evht->handlers[ET_SERVER_HB_REQ] = ev_handle_server_hb_req;
	se.el->evht->handlers[ET_SERVER_HB_RSP] = ev_handle_server_hb_resp;
	se.el->evht->handlers[ET_SERVER_BC_REQ] = ev_handle_server_bc_req;
	se.el->evht->handlers[ET_SERVER_CHECK_HB] = ev_handle_server_check_hb;
	se.el->evht->handlers[ET_SERVER_SRV_REQ] = ev_handle_server_srv_req;


	se.el->evht->handlers[ET_CLIENT_POWER_ON] = ev_handle_client_power_on;
	se.el->evht->handlers[ET_CLIENT_HB_REQ] = ev_handle_client_hb_req;
	se.el->evht->handlers[ET_CLIENT_BC_REQ] = ev_handle_client_bc_req;
	se.el->evht->handlers[ET_CLIENT_SWITCHING] = ev_handle_client_switching;
	se.el->evht->handlers[ET_CLIENT_SRV_REQ] = ev_handle_client_srv_req;


	se.nclients = CLIENT_COUNT;
	//mmm: start end of server not set
	se.server = sm_server_create();

	//se.nsrvreq = 0;
	

	/*--- add clients ---*/
	printf("[GEN_CLIENTS]started.\n");
	clients = generate_input_clients_simple_1(se.nclients, SWITCHING_COUNT, CHANNEL_COUNT);
	printf("[GEN_CLIENTS]ended.\n");

	se.clients = (sm_client**)malloc(se.nclients*sizeof(sm_client*));
	for(i=0;i<se.nclients;i++)	{
		se.clients[i] = clients+i;
	}

	//printf("simenvinit 1\n");

	//mmm: not implemented yet!
	/*-- add client poweron event--*/

	printf("[ADD_PLANED_EVENTS]started.\n");
	printf("[ADD_PLANED_EVENTS]step 1. add client switching events.\n");
	/*-- add client channel change event--*/
	end = 0;
	for(i=0;i<se.nclients;i++)	{
		//printf("client %d\n", i);
	//printf("simenvinit 1.5, i=%d\n", i);
		client = se.clients[i];
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
//			printf("ltemp=%ld\n", ltemp);
			ltemp += client->plan->switchings[j]->duration;

			if(ltemp>end)	{
				end = ltemp;
			}

			ev_list_add(se.el->evlist, e);
//			printf("evlistsize=%d, added e time=%ld, lasttime=%ld\n", se.el->evlist->size, e->time, (ev_list_get(se.el->evlist, se.el->evlist->size-1))->time );
		}
	}

	//mmmm:temp, why so slow???
	/*
	for(i=0;i<se.el->evlist->size;i++)	{
		e = ev_list_get(se.el->evlist, i);
		//printf("in sim1, time=%ld\n", e->time);
	}
	*/

	//printf("simenvinit 2\n");
	printf("[ADD_PLANED_EVENTS]step 2. add client heartbeat events.\n");
	/*-- add client heartbeat event--*/
	ltemp = HEARTBEAT_INTERVAL;	//mmm: hb interval, should be loaded
	for(i=0;i<se.nclients;i++)	{
		client = se.clients[i];
		for(j=client->plan->arrival+ltemp;j<client->plan->departure;j+=ltemp)	{	//mmm: check whether departure time is right
			e = ev_create(ET_CLIENT_HB_REQ, j);
			ed_chr = (evdata_client_hb_req*)malloc(sizeof(evdata_client_hb_req));
			e->data = (void*)ed_chr;
			e->agent = (void*)client;

			ev_list_add(se.el->evlist, e);
		}
	}

	//printf("simenvinit 3\n");
	/*-- add server broadcast event--*/
	printf("[ADD_PLANED_EVENTS]step 3. add server broadcast events.start = %ld, end = %ld, step = %ld\n", se.server->start, end, ltemp);
	ltemp = BROADCAST_INTERVAL;	//mmm: bc interval, should be loaded
	se.server->end = end;
	for(i=se.server->start+ltemp;i<se.server->end;i+=ltemp)	{	//mmm: < should be <=?
		//printf("[debug]i=%d\n", i);
		e = ev_create(ET_SERVER_BC_REQ, i);
		ed_sbr = (evdata_server_bc_req*)malloc(sizeof(evdata_server_bc_req));
		e->data = (void*)ed_sbr;
		e->agent = (void*)se.server;

		ev_list_add(se.el->evlist, e);
	}
	//printf("simenvinit end, server.cisize=%d\n", se.server->ci->size);

	//mmmm:temp
	printf("[ADD_PLANED_EVENTS]end\n");
	/*
	for(i=0;i<se.el->evlist->size;i++)	{
		e = ev_list_get(se.el->evlist, i);
		//printf("in sim, time=%ld\n", e->time);
	}
	*/
}


/* ------ client info related ops -----------*/
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

/* ------ config info related ops -----------*/
void* sim_env_get_config(int configid)
{
	int i;
	sim_config* cfg;
	int type;
	void* v;


	cfg = se.config;
	for(i=0;i<CONFIG_TYPE_INFO_SIZE;i++)	{
		if(cfg->types[i].id == configid)	{
			break;
		}
	}

	if(i!=CONFIG_TYPE_INFO_SIZE)	{
		type = cfg->types[i].type;
	}
	else	{
		printf("error getting config (configid=%d): not found!\n", configid);
		return NULL;
	}

	switch(type)	{
		case VALUE_TYPE_INT:
			v = (void*)&cfg->val[configid].i;
			break;
		case VALUE_TYPE_LONG:
			v = (void*)&cfg->val[configid].l;
			break;
		case VALUE_TYPE_CHARARRAY:
			v = cfg->val[configid].s;
			break;
		default:
			v = NULL;
			break;
	}

	return v;
}


int sim_env_set_config(int configid, void* v)
{
	sim_config* cfg;
	int type;
	int i;
	value* val;

	cfg = se.config;
	for(i=0;i<CONFIG_TYPE_INFO_SIZE;i++)	{
		if(cfg->types[i].id == configid)	{
			break;
		}
	}

	if(i!=CONFIG_TYPE_INFO_SIZE)	{
		type = cfg->types[i].type;
	}
	else	{
		printf("error setting config (configid=%d): config not found!\n", configid);
		return -1;
	}

	val = &cfg->val[configid];
	
	switch(type)	{
		case VALUE_TYPE_INT:
			val->i = *((int*)v);
			break;
		case VALUE_TYPE_LONG:
			val->l = *((long*)v);
			break;
		case VALUE_TYPE_CHARARRAY:
			strcpy(val->s, (char*)v);	//mmm: no size ok?
			break;
		default:
			break;
	}

	return 0;
}

/* ------ stat related ops -----------*/
void	sim_env_stat_init()
{
	int i;
	se.stat = (sim_stats*)malloc(sizeof(sim_stats));
	for(i=0;i<STAT_SIZE;i++)	{
		se.stat->val[i] = 0;
	}
}

int sim_env_stat_set(int statid, long v)
{
	se.stat->val[statid] = v;
	return 0;
}

long sim_env_stat_get(int statid)
{
	//mmm: check statid if maximum
	return se.stat->val[statid];
}

int sim_env_stat_inc(int statid)
{
	long v;
	v = se.stat->val[statid];
	se.stat->val[statid] = v+1;
	//mmm: check max
	return 0;
}
