#include "client.h"

#include <stdio.h>
#include <stdlib.h>

channel_client* channel_info_client_get(channel_info_client* ci, int chid)
{
	int i;
	for(i=0;i<ci->size;i++)	{
		if(ci->chlist[i].chid == chid)	{
			return &ci->chlist[i];
		}
	}
	
	return NULL;
}

void channel_info_client_update(channel_info_client* ci, channel_client* chlist, int size, int mode)
{
	if(mode == CHANNEL_INFO_UPDATE_MODE_OVERWRITE)	{
		ci->chlist = chlist;
		ci->size = size;
	}
}

/*------- event handler--------------------------*/
void client_handle_chchange(client* c, int chid, long time)
{
	channel_client* ch;
	ch = channel_info_client_get(c->ci, chid);
	if(ch==NULL)	{
		//mmm:
		printf("[client] ch not exist, send req explicitly!\nMMM: ADD SERVE REQUEST EVENT!!!\n");
	}
	else	{
		printf("[client] watching!\n");
		c->chid = chid;
	}
}
