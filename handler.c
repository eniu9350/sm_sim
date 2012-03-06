#include "handler.h"

#include "common.h"

void handle_hbreq(ev_loop* el,ev* e)
{
	//mmm: tmp
	context_global* gctx;
	printf("GCTXNULL@handle_hbreq!!!!!!!!!!!!!!!!!!\n");

	//save request

	hbreq_list* buf = gctx->server->hbreq_buf;
	if(buf->size==buf->capacity)	{
		printf("buf capacity exceeded @ handle_hbreq!!!!!!!!!!!\n");
	}
	buf->list[buf->size] = e->data;
	buf->size = buf->size + 1;
	
}

void handle_hbreq_client(ev_loop* el,ev* e)
{
	ev* newe = (ev*)malloc(sizeof(ev));
	newe->type = ET_HB_REQ;
	newe->time = e->time + T_TRAN;
	newe->data = e->data;
	newe->agent = e->agent;

	fire_event(el->evlist, newe);
}

void handle_hbrsp(ev_loop* el,ev* e);
void handle_bcreq(ev_loop* el,ev* e);
void handle_bcrsp(ev_loop* el,ev* e);
void handle_srvreq(ev_loop* el,ev* e);
