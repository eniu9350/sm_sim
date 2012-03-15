#include "ch.h"



#include <stdio.h>
#include <stdlib.h>

/*------- channel ops --------------------------*/
int ch_join(ch* ch, int uid)
{
	int i;
	//mmm: check if uid exist yet
	userlist_add(ch->users, uid);
	return 0;
}

int ch_leave(ch* ch, int uid)
{
	int i;
	//mmm: check if uid exist
	userlist_remove_by_id(ch->users, uid);
	return 0;
}


void ch_info_get_update_list(ch_info* ci, int* uidlist, int* chlist, int n, ch_update** culist, int* nculist)
{
	char* flag;	
	int* pre;//previous channel
	int i;
	ch* ch;
	int tmpch;
	//channel_update** culist;
	//int nculist;
	ch_update* cu;
	//0:not processed, 1:removal processed, 2:add processed, 3: all
	flag = (char*)malloc(n*sizeof(char));
	pre = (int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)	{
		flag[i] = 0;
	}
	for(i=0;i<n;i++)	{
		ch = ch_info_get_by_uid(ci, uidlist[i]);
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

		cu = ch_update_create();
		culist[*nculist] = cu;
		*nculist = *nculist+1;
		for(i=0;i<n;i++)	{
			if(flag[i]==0 || flag[i]==2)	{
				//choose one not processed leave
				flag[i] += 1;	//mmm: should be &1
				ch_update_add_leave(cu, uidlist[i]);
				cu->chid = pre[i];
				break;
			}
		}

		if(i!=n)	{
			for(i=0;i<n;i++)	{//add other unprocessed leave same as cu->chid
				if((flag[i]==0 || flag[i]==2) && pre[i] == cu->chid)	{
					ch_update_add_leave(cu, uidlist[i]);
					flag[i] += 1;
				}
			}
		}
		else	{//no upprocessed leave
			for(i=0;i<n;i++)	{//choose an unprocessed join
				if(flag[i]==0 || flag[i] == 2)	{
					flag[i] += 2;
					cu->chid = chlist[i];
					ch_update_add_join(cu, uidlist[i]);
				}
			}

			for(i=0;i<n;i++)	{//choose (other/all) unprocessed join
				if((flag[i]==0 || flag[i]==1) && chlist[i] == cu->chid)	{
					flag[i] += 2;
					ch_update_add_join(cu, uidlist[i]);
				}
			}
		}
	}

	;
	//mmm

}


ch* ch_info_get_by_uid(ch_info* ci, int uid)
{
	int i,j;
	for(i=0;i<ci->size;i++)	{
		if(ci->chlist[i]->users==NULL)	{
			perror("userlist in ch null!\n");
			return NULL;
		}
		for(j=0;j<ci->chlist[i]->users->size;j++)	{
			if(ci->chlist[i]->users->list[j] == uid)	{
				return ci->chlist[i];
			}
		}
	}

	printf("NEW UID!\n");
	return NULL;
}

ch* ch_info_get_by_sgid_and_chid(ch_info* ci, int sgid, int chid)
{
	int i;
	for(i=0;i<ci->size;i++)	{
		if(ci->chlist[i]->sgid == sgid && ci->chlist[i]->chid == chid)	{
			return ci->chlist[i];
		}
	}
	return NULL;
}

/*------- chinfo ops --------------------------*/
ch_info* ch_info_create()
{
	ch_info* ci;
	ci = (ch_info*)malloc(sizeof(ch_info));
	ci->capacity = 200;
	ci->size = 0;
	ci->chlist = (ch**)malloc(ci->capacity*sizeof(ch*));
	return ci;
}

void ch_info_add(ch_info* ci, ch* c)
{
	ch** newchlist;
	int newcapacity;

	if(ci->size==ci->capacity)	{
		newcapacity = ci->capacity*2;
		if(newcapacity==2000)	{
			perror("max capacity exceeded in ch_info_add!\n");
			return;
		}
		else	{
			newchlist = (ch**)realloc(ci->chlist, newcapacity*sizeof(ch*));
			if(!newchlist)	{
				perror("expand error in ch_info_add\n");
			}
			else	{
				ci->chlist = newchlist;
				ci->capacity = newcapacity;
			}
		}
	}

	ci->chlist[ci->size] = c;
	ci->size = ci->size+1;
}


ch_info_client* ch_info_client_create()
{
	ch_info_client* ci;
	ci = (ch_info_client*)malloc(sizeof(ch_info_client));
	ci->size = 0;
	ci->chlist = NULL;	//mmm: right?
	return ci;
}

/*------- channel_update ops --------------------------*/
ch_update* ch_update_create()
{
	ch_update* cu;
	cu = (ch_update*)malloc(sizeof(ch_update));
	cu->leave = userlist_create();
	cu->join = userlist_create();
	cu->processed = 0;
}

void ch_update_add_join(ch_update* cu, int uid)
{
	userlist_add(cu->join, uid);
}

void ch_update_add_leave(ch_update* cu, int uid)
{
	userlist_add(cu->leave, uid);
}
