#include "ch.h"



#include <stdio.h>
#include <stdlib.h>

/*------- channel ops --------------------------*/
int channel_join(channel* ch, int uid)
{
	int i;
	//mmm: check if uid exist yet
	userlist_add(ch->users, uid);
	return 0;
}

int channel_leave(channel* ch, int uid)
{
	int i;
	//mmm: check if uid exist
	userlist_remove_by_id(ch->users, uid);
	return 0;
}


void channel_info_get_update_list(channel_info* ci, int* uidlist, int* chlist, int n, channel_update** culist, int* nculist)
{
	char* flag;	
	int* pre;//previous channel
	int i;
	channel* ch;
	int tmpch;
	//channel_update** culist;
	//int nculist;
	channel_update* cu;
	//0:not processed, 1:removal processed, 2:add processed, 3: all
	flag = (char*)malloc(n*sizeof(char));
	pre = (int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)	{
		flag[i] = 0;
	}
	for(i=0;i<n;i++)	{
		ch = channel_info_get_by_uid(ci, uidlist[i]);
		if(ch == NULL)	{
			perror("chid NULL when batch processing update!\n");
			continue;
		}
		pre[i] = ch->chid;
	}

	//culist = (channel_update**)malloc(n*sizeof(channel_update*));
	*nculist = 0;

	while(1)	{
		for(i=0;i<n;i++)	{
			if(flag[i] != 3)	{
				break;
			}
		}

		if(i==n)	{
			break;
		}

		cu = channel_update_create();
		culist[*nculist] = cu;
		*nculist = *nculist+1;
		for(i=0;i<n;i++)	{
			if(flag[i]==0 || flag[i]==2)	{
				//choose one not processed leave
				flag[i] += 1;	//mmm: should be &1
				channel_update_add_leave(cu, uidlist[i]);
				cu->chid = pre[i];
				break;
			}
		}

		if(i!=n)	{
			for(i=0;i<n;i++)	{//add other unprocessed leave same as cu->chid
				if((flag[i]==0 || flag[i]==2) && pre[i] == cu->chid)	{
					channel_update_add_leave(cu, uidlist[i]);
					flag[i] += 1;
				}
			}
		}
		else	{//no upprocessed leave
			for(i=0;i<n;i++)	{//choose an unprocessed join
				if(flag[i]==0 || flag[i] == 2)	{
					flag[i] += 2;
					cu->chid = chlist[i];
					channel_update_add_join(cu, uidlist[i]);
				}
			}

			for(i=0;i<n;i++)	{//choose (other/all) unprocessed join
				if((flag[i]==0 || flag[i]==1) && chlist[i] == cu->chid)	{
					flag[i] += 2;
					channel_update_add_join(cu, uidlist[i]);
				}
			}
		}
	}

	;
	//mmm

}


channel* channel_info_get_by_uid(channel_info* ci, int uid)
{
	int i,j;
	for(i=0;i<ci->size;i++)	{
		if(ci->chlist[i].users==NULL)	{
			perror("userlist in ch null!\n");
			return NULL;
		}
		for(j=0;j<ci->chlist[i].users->size;j++)	{
			if(ci->chlist[i].users->list[j] == uid)	{
				return &(ci->chlist[i]);
			}
		}
	}

	printf("NEW UID!\n");
	return NULL;
}

channel* channel_info_get_by_sgid_and_chid(channel_info* ci, int sgid, int chid)
{
	int i;
	for(i=0;i<ci->size;i++)	{
		if(ci->chlist[i].sgid == sgid && ci->chlist[i].chid == chid)	{
			return &(ci->chlist[i]);
		}
	}
	return NULL;
}

/*------- channel_update ops --------------------------*/
channel_update* channel_update_create()
{
	channel_update* cu;
	cu = (channel_update*)malloc(sizeof(channel_update));
	cu->leave = userlist_create();
	cu->join = userlist_create();
	cu->processed = 0;
}

void channel_update_add_join(channel_update* cu, int uid)
{
	userlist_add(cu->join, uid);
}

void channel_update_add_leave(channel_update* cu, int uid)
{
	userlist_add(cu->leave, uid);
}
