#include "ev_handler.h"

//#include "common.h"
//#include "ch.h"

#include "sim_env.h"
#include <stdlib.h>
#include <stdio.h>
#include "ch.h"

extern sim_env se;	//mmm: trival --- right?

/*------- server event handler--------------------------*/
void ev_handle_server_hb_req(ev_loop* el,ev* e)
{
	//printf("ev_handle_server_hb_req\n");
	
	//mmm: tmp
	//context_global* gctx;
	//printf("ENV NULL@handle_hbreq!!!!!!!!!!!!!!!!!!\n");

	//save request

	//mmmmmm: should purge reqbuf each round!
	
	evdata_server_hb_req_alist* buf = se.server->buf_hb_req;
	//mmm: check e type
	add_evdata_server_hb_req(buf, e->data);
	/*
	if(buf->size==buf->capacity)	{
	printf("buf capacity exceeded @ handle_hbreq!!!!!!!!!!!\n");
	}
	buf->list[buf->size] = e->data;
	buf->size = buf->size + 1;
	*/
}

void ev_handle_server_hb_resp(ev_loop* el,ev* e)
{
	//mmm: not implemented yet
	//printf("ev_handle_server_hb_resp\n");
}

void ev_handle_server_bc_req(ev_loop* el, ev* e)
{
	//printf("ev_handle_server_bc_req\n");
	sm_server* server;
	sm_client* client;
	ev* newe;
	evdata_client_bc_req* ed;
	ch_client** chlist;	//chlist sent to client
	long ltemp;
	int i;

	server = se.server;

	chlist = (ch_client**)malloc(server->ci->size*sizeof(ch_client*));
	for(i=0;i<server->ci->size;i++)	{
		chlist[i] = (ch_client*)malloc(sizeof(ch_client));
		chlist[i]->chid = server->ci->list[i]->chid;
		chlist[i]->f = server->ci->list[i]->f;	//mmm: should copy?
	}

	ed = (evdata_client_bc_req*)malloc(sizeof(evdata_client_bc_req));
	ed->chlist = chlist;
	ed->size = server->ci->size;

	//mmm: constant server bc req -> client bc req	
	ltemp = 1;


	for(i=0;i<se.nclients;i++)	{
		client = se.clients[i];
		if(client->state != CLIENT_STATE_END)	{
			newe = ev_create(ET_CLIENT_BC_REQ, el->now+1);
			newe->data = (void*)ed;
			newe->agent = (void*)client;
			ev_list_add(el->evlist, newe);
		}
	}
}



void ev_handle_server_check_hb(ev_loop* el, ev* e)
{
	ch_update** culist;
	int nculist = 0;
	int* uidlist;
	int* chlist;
	int n;
	ch* c;
	int i,j;
	sm_server* server;
	ev* newe;
	evdata_server_ra_req* ed_ra;
	evdata_server_rr_req* ed_rr;
	long ltemp;
	ch* c2;

	//printf("ev_handle_server_check_hb\n");

	culist = (ch_update**)malloc(1000*sizeof(ch_update*));	//mmm: large enough?

	server = (sm_server*)e->agent;	
	n = server->buf_hb_req->size;
	uidlist = (int*)malloc(n*sizeof(int));
	chlist = (int*)malloc(n*sizeof(int));


	for(i=0;i<n;i++)	{
		uidlist[i] = server->buf_hb_req->list[i]->uid;
		chlist[i] = server->buf_hb_req->list[i]->chid;
	}

	//get update list
	ch_alist_get_update_list(server->ci, uidlist, chlist, n, culist, &nculist);

	//check res alloc and res rel
	for(i=0;i<nculist;i++)	{
		if(culist[i]->leave->size == 0)	{
			c2 = ch_alist_get_by_sgid_and_chid(server->ci, culist[i]->sgid, culist[i]->chid);
			if(c2 == NULL)	{//alloc res
				culist[i]->processed = 1;
				//mmm: should log
				ltemp = 1;	//mmm: constant alloc -> ra req
				newe = ev_create(ET_SERVER_RA_REQ, el->now+ltemp);
				newe->agent = (void*)server;
				ed_ra = (evdata_server_ra_req*)malloc(sizeof(evdata_server_ra_req));
				ed_ra->sgid = culist[i]->sgid;
				ed_ra->chid = culist[i]->chid;
				newe->data = ed_ra;
				ev_list_add(el->evlist, newe);

				//mmm: assume res allocated immediately
				//add new ch (including users)
				c2 = (ch*)malloc(sizeof(ch));
				c2->sgid = culist[i]->sgid;
				c2->chid = culist[i]->chid;
				c2->users = create_user_alist();
				for(j=0;j<culist[i]->join->size;j++)	{	//mmm: could the size be 0?
					add_user(c2->users, culist[i]->join->list[j]);
				}
				add_ch(server->ci, c2);
			}
			else	{
				for(j=0;j<culist[i]->join->size;j++)	{	//mmm: could the size be 0?
					add_user(c2->users, culist[i]->join->list[j]);
				}
			}
		}

		if(culist[i]->join->size == 0)	{
			printf("join size=0\n");
			c = ch_alist_get_by_sgid_and_chid(server->ci, culist[i]->sgid, culist[i]->chid);
			//mmm: heuristics here, right?(forcing a check will gurantee ok)
			if(c->users->size == culist[i]->leave->size)	{//rel res
				culist[i]->processed = 1;
				//mmm: should log
				newe = (ev*)malloc(sizeof(ev));
				newe->type = ET_SERVER_RR_REQ;
				newe->agent = server;
				ed_rr = (evdata_server_rr_req*)malloc(sizeof(evdata_server_rr_req));
				ed_rr->sgid = culist[i]->sgid;
				ed_rr->chid = culist[i]->chid;
				newe->data = ed_rr;
				ev_list_add(el->evlist, newe);

				for(j=0;j<server->ci->size;j++)	{
					//					remove_ch(server->ci, c-&server->ci->list[0]);	//mmmmmm: right?
					if(server->ci->list[j] == c)	{
						remove_ch(server->ci, j);
						printf("remove ok\n");
						break;
					}
				}
				if(j==server->ci->size)	{
					perror("remove ch failed\n");
				}
			}
		}
	}//end for

	for(i=0;i<nculist;i++)	{
		if(culist[i]->processed==0)	{	//just modify data
			c = ch_alist_get_by_sgid_and_chid(server->ci, culist[i]->sgid, culist[i]->chid);
			for(j=0;j<culist[i]->join->size;j++)	{
				ch_join(c, culist[i]->join->list[j]);
			}
			for(j=0;j<culist[i]->leave->size;j++)	{
				ch_leave(c, culist[i]->leave->list[j]);
			}
		}
	}
}

void ev_handle_server_srv_req(ev_loop* el, ev* e)
{

	ch* c;
	evdata_server_srv_req* ed = (evdata_server_srv_req*)e->data;
	sm_client* client;
	int* u;

	//log
	sim_env_stat_inc(STAT_ID_SERVER_SRV_REQ_COUNT);	
	//se.nsrvreq++;

	client = sim_env_get_client_by_uid(ed->uid);

	//printf("ev_handle_server_srv_req\n");

	c = ch_alist_get_by_sgid_and_chid(se.server->ci, client->sgid, ed->chid);
	if(c==NULL)	{
		//mmmm: alloc res first
		c = ch_create(client->sgid, ed->chid);
		//mmm: no freq info yet
		add_ch(se.server->ci, c);
	}
	else	{
		//printf("chinfo: sgid=%d, chid=%d, cisize=%d, reqchid=%d\n", c->sgid, c->chid, se.server->ci->size, ed->chid);
		u = (int*)malloc(sizeof(int));
		*u = ed->uid;
		add_user(c->users, u);
	}

	//mmmm: should generate response
}
/*------- client event handler--------------------------*/
void ev_handle_client_power_on(ev_loop* el, ev* e)
{

	//printf("ev_handle_client_power_on\n");
}

void ev_handle_client_hb_req(ev_loop* el,ev* e)
{
	//printf("ev_handle_client_hb_req\n");
	long ltemp;
	ev* newe;
	evdata_server_hb_req* ed;
	sm_client* client;

	client = (sm_client*)e->agent;
	ltemp = 1;	//mmm: constant client hb -> server hb
	newe = ev_create(ET_SERVER_HB_REQ, el->now+ltemp);
	ed = (evdata_server_hb_req*)malloc(sizeof(evdata_server_hb_req));
	ed->uid = client->id;
	ed->chid = client->chid;	//mmm: any change after that?
	newe->data = (void*)ed;
	newe->agent = (void*)se.server;

	ev_list_add(el->evlist, newe);

	/*
		 ev* newe = (ev*)malloc(sizeof(ev));
		 newe->type = ET_HB_REQ;
		 newe->time = e->time + T_TRAN;
		 newe->data = e->data;
		 newe->agent = e->agent;

		 fire_event(el->evlist, newe);
		 */
}

void ev_handle_client_bc_req(ev_loop* el, ev* e)
{
	//printf("ev_handle_server_bc_req\n");
	sm_client* client;
	evdata_client_bc_req* ed;	

	ed = (evdata_client_bc_req*)e->data;

	client = (sm_client*)e->agent;

	//update local ch list
	client->ci->chlist = ed->chlist;
	client->ci->size = ed->size;
}

void ev_handle_client_switching(ev_loop* el, ev* e)
{
	//printf("ev_handle_client_switching\n");
	evdata_client_switching* ed_cs;
	sm_client* client;
	ev* newe;
	evdata_server_srv_req* ed_csr;
	int chid;
	int i;
	long ltemp;
	client = (sm_client*)e->agent;
	ed_cs = (evdata_client_switching*)e->data;
	chid = ed_cs->chid;

	for(i=0;i<client->ci->size;i++)	{
		if(client->ci->chlist[i]->chid == chid)	{
			break;
		}
	}

	if(i==client->ci->size) {	//chid not in local channel list, need to send request
		//mmm: constant---- switching->channelchange@server
		ltemp = 1;
		newe = ev_create(ET_SERVER_SRV_REQ, el->now+ltemp); 
		ed_csr = (evdata_server_srv_req*)malloc(sizeof(evdata_client_srv_req));
		ed_csr->uid = client->id;
		ed_csr->chid = chid;
		newe->data = (void*)ed_csr;
		newe->agent = (void*)client;

		ev_list_add(el->evlist, newe);
	}
	else	{	// chid in local channel list, no need to send request, just change current chid
		client->chid = chid;
		//mmm: some logging(indicating that the user starts receiving media stream of some channel)
	}
}

void ev_handle_client_srv_req(ev_loop* el, ev* e)	{	//mmmm: server srv req should be added indirectly through this event
	evdata_server_srv_req* ed;
	ev* newe;
	sm_client* client;

	client = (sm_client*)e->agent;

	newe = ev_create(ET_SERVER_SRV_REQ, e->time+1); //mmm: time should be paramed	
	ed = (evdata_server_srv_req*)malloc(sizeof(evdata_server_srv_req));
	ed->uid = client->id;
	ed->chid = ((evdata_client_srv_req*)e->data)->chid;
	newe->data = ed;
	newe->agent = (void*)se.server;

	ev_list_add(el->evlist, newe);

}




void handle_hbrsp(ev_loop* el,ev* e)
{
}

void handle_bcreq(ev_loop* el,ev* e)
{
}

void handle_bcrsp(ev_loop* el,ev* e)
{
}

void handle_srvreq(ev_loop* el,ev* e)
{
}

