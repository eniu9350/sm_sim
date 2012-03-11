#include "client.h"

#include <stdio.h>
#include <stdlib.h>

//#include "event.h"

ch_client* ch_info_client_get(ch_info_client* ci, int chid)
{
	int i;
	for(i=0;i<ci->size;i++)	{
		if(ci->chlist[i].chid == chid)	{
			return &ci->chlist[i];
		}
	}
	
	return NULL;
}

void ch_info_client_update(ch_info_client* ci, ch_client* chlist, int size, int mode)
{
	if(mode == CHANNEL_INFO_UPDATE_MODE_OVERWRITE)	{
		ci->chlist = chlist;
		ci->size = size;
	}
}

/*------- event handler--------------------------*/
//void client_handle_chchange(client* c, int chid, long time)
void client_handle_chchange(ev* e)
{
	client* c;	
	int chid;
	ch_client* ch;
	ch = ch_info_client_get(c->ci, chid);
	ev* newe;
	evdata_srvreq* edata;
	context_global* gctx;

	c = (client*)e->agent;
	chid = ((evdata_chchange*)e->data)->ch;

	gctx = c->gctx;
	if(ch==NULL)	{
		//mmm:
		printf("[client] ch not exist, send req explicitly!\nMMM: ADD SERVE REQUEST EVENT!!!\n");
		edata = (evdata_srvreq*)malloc(sizeof(evdata_srvreq));
		edata->uid = c->id;
		edata->ch = chid;
		newe = ev_create(ET_SRV_REQ, gctx->now);	//mmm: should be now+1?
		newe->data = edata;
		newe->agent = c;
		ev_list_add(gctx->el->evlist, newe);
	}
	else	{
		printf("[client] watching!\n");
		c->chid = chid;
	}
}

void client_handle_bcreq(ev* e)
{
	client* c;	
	evdata_bcreq* edata;

	c = (client*)e->agent;
	edata = (evdata_bcreq*)e->data;

	ch_info_client_update(c->ci,  edata->chlist, edata->chsize, CHANNEL_INFO_UPDATE_MODE_OVERWRITE);
}
