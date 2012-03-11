#include "ev_handler.h"

//#include "common.h"
//#include "ch.h"

#include "sim_env.h"

void handle_server_hb_req(ev_loop* el,ev* e)
{
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
}

void ev_handle_server_hb_resp(ev_loop* el,ev* e)
{
//mmm: not implemented yet
}

/*------- client event handler--------------------------*/
void ev_handle_client_power_on(ev_loop* el, ev* e)
{
	
}

void ev_handle_client_hb_req(ev_loop* el,ev* e)
{
	ev* newe = (ev*)malloc(sizeof(ev));
	newe->type = ET_HB_REQ;
	newe->time = e->time + T_TRAN;
	newe->data = e->data;
	newe->agent = e->agent;

	fire_event(el->evlist, newe);
}

void ev_handle_server_check_hb(ev_loop* el, ev* e)
{
	ch_update** culist;
	int nculist = 0;
	int* uidlist;
	int* chlist;
	int n;
	ch* ch;
	int i;
	server* s;
	ev* e;
	evdata_rareq* evdata;

	culist = (ch_update**)malloc(1000*sizeof(ch_update*));	//mmm: large enough?

	s = (server*)e->agent;	
	n = s->hbreq_buf->size;
	uidlist = (int*)malloc(n*sizeof(int));
	chlist = (int*)malloc(n*sizeof(int));


	for(i=0;i<n;i++)	{
		uidlist[i] = s->hbreq_buf->list[i]->uid;
		chlist[i] = s->hbreq_buf->list[i]->ch;
	}

	//get update list
	ch_info_get_update_list(s->ci, uidlist, chlist, n, culist, &nculist);

	//check res alloc and res rel
	for(i=0;i<nculist;i++)	{
		if(culist[i]->leave->size == 0)	{
			if(ch_info_get_by_sgid_and_chid(s->ci, culist[i]->sgid, culist[i]->chid) == NULL)	{//alloc res
				culist[i]->processed = 1;
				//mmm: should log
				e = (ev*)malloc(sizeof(ev));
				e->type = ET_RA_REQ;
				e->agent = server;
				evdata = (evdata_rareq*)malloc(sizeof(evdata_rareq));
				evdata->sgid = culist[i]->sgid;
				evdata->chid = culist[i]->chid;
				e->data = evdata;
				fire_event(el->evlist, evdata);
			}
		}

		if(culist[i]->join->size == 0)	{
			ch = ch_info_get_by_sgid_and_chid(s->ci, culist[i]->sgid, culist[i]->chid);
			//mmm: heuristics here, right?(forcing a check will gurantee ok)
			if(ch->users->size == culist[i]->leave->size)	{//rel res
				culist[i]->processed = 1;
				//mmm: should log
				e = (ev*)malloc(sizeof(ev));
				e->type = ET_RR_REQ;
				e->agent = server;
				evdata = (evdata_rrreq*)malloc(sizeof(evdata_rrreq));
				evdata->sgid = culist[i]->sgid;
				evdata->chid = culist[i]->chid;
				e->data = evdata;
				fire_event(el->evlist, evdata);
			}
		}
	}//end for

	for(i=0;i<nculist;i++)	{
		if(culist[i]->processed==0)	{	//just modify data
			ch = ch_info_get_by_sgid_and_chid(s->ci, culist[i]->sgid, culist[i]->chid);
			for(j=0;j<culist[i]->join->size;j++)	{
				ch_join(ch, culist[i]->join->list[j]);
			}
			for(j=0;j<culist[i]->leave->size;j++)	{
				ch_leave(ch, culist[i]->leave->list[j]);
			}
		}
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

