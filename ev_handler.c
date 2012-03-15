#include "ev_handler.h"

//#include "common.h"
//#include "ch.h"

#include "sim_env.h"
#include <stdlib.h>
#include <stdio.h>

extern sim_env se;	//mmm: trival --- right?

/*------- server event handler--------------------------*/
void ev_handle_server_hb_req(ev_loop* el,ev* e)
{
	printf("ev_handle_server_hb_req\n");
	/*
	//mmm: tmp
	//context_global* gctx;
	printf("ENV NULL@handle_hbreq!!!!!!!!!!!!!!!!!!\n");

	//save request

	hbreq_list* buf = gctx->server->hbreq_buf;
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
	printf("ev_handle_server_hb_resp\n");
}

void ev_handle_server_bc_req(ev_loop* el, ev* e)
{
	printf("ev_handle_server_bc_req\n");
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
		chlist[i]->chid = server->ci->chlist[i]->chid;
		chlist[i]->f = server->ci->chlist[i]->f;	//mmm: should copy?
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
	printf("ev_handle_server_check_hb\n");
	ch_update** culist;
	int nculist = 0;
	int* uidlist;
	int* chlist;
	int n;
	ch* ch;
	int i,j;
	sm_server* server;
	ev* newe;
	evdata_server_ra_req* ed_ra;
	evdata_server_rr_req* ed_rr;
	long ltemp;

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
	ch_info_get_update_list(server->ci, uidlist, chlist, n, culist, &nculist);

	//check res alloc and res rel
	for(i=0;i<nculist;i++)	{
		if(culist[i]->leave->size == 0)	{
			if(ch_info_get_by_sgid_and_chid(server->ci, culist[i]->sgid, culist[i]->chid) == NULL)	{//alloc res
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
			}
		}

		if(culist[i]->join->size == 0)	{
			ch = ch_info_get_by_sgid_and_chid(server->ci, culist[i]->sgid, culist[i]->chid);
			//mmm: heuristics here, right?(forcing a check will gurantee ok)
			if(ch->users->size == culist[i]->leave->size)	{//rel res
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
			}
		}
	}//end for

	for(i=0;i<nculist;i++)	{
		if(culist[i]->processed==0)	{	//just modify data
			ch = ch_info_get_by_sgid_and_chid(server->ci, culist[i]->sgid, culist[i]->chid);
			for(j=0;j<culist[i]->join->size;j++)	{
				ch_join(ch, culist[i]->join->list[j]);
			}
			for(j=0;j<culist[i]->leave->size;j++)	{
				ch_leave(ch, culist[i]->leave->list[j]);
			}
		}
	}
}

void ev_handle_server_srv_req(ev_loop* el, ev* e)
{
}
/*------- client event handler--------------------------*/
void ev_handle_client_power_on(ev_loop* el, ev* e)
{

	printf("ev_handle_client_power_on\n");
}

void ev_handle_client_hb_req(ev_loop* el,ev* e)
{
	printf("ev_handle_client_hb_req\n");
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
	printf("ev_handle_server_bc_req\n");
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
	printf("ev_handle_client_switching\n");
	evdata_client_switching* ed_cs;
	sm_client* client;
	ev* newe;
	evdata_client_srv_req* ed_csr;
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
		//mmm: constant---- switching->channelchange
		ltemp = 1;
		newe = ev_create(ET_SERVER_SRV_REQ, el->now+ltemp); 
		ed_csr = (evdata_client_srv_req*)malloc(sizeof(evdata_client_srv_req));
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

