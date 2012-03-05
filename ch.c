#include "ch.h"

#include <stdio.h>
#include <stdlib.h>

/*------- userlist ops --------------------------*/
userlist* userlist_create()
{
	userlist* ul;
	ul = (userlist*)malloc(sizeof(userlist));
	ul->size = 0;
	ul->capacity = 100;
	ul->list = (int*)malloc(100*sizeof(int));
	return ul;
}


int userlist_add(userlist* ul, int uid)
{
	int* old;
	old = ul->list;
	if(ul->size==ul->capacity)	{
		if(userlist_expand(ul) == NULL)	{
			return -1;
		}
	}

	ul->list[ul->size] = uid;
	ul->size = ul->size+1;

	return 0;
}


int userlist_expand(userlist* ul)
{
	ul->list = (int*)realloc(ul->list, ul->capacity * 2);
	if(ul->list==NULL)	{
		perror("realloc failed when expanding userlist\n");
		return -1;
	}
	ul->capacity = ul->capacity*2;
	return 0;
}


/*------- ch ops --------------------------*/
channel_info_update_batch(channel_info* ci, int* uidlist, int* chlist, int n)
{
	char* flag;	
	int* pre;//previous channel
	int i;
	channel* ch;
	int tmpch;
	channel_update** culist;
	int nculist;
	channel_update* cu;
	//0:not processed, 1:removal processed, 2:add processed, 3: all
	flag = (char*)malloc(n*sizeof(char));
	pre = (int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)	{
		flag[i] = 0;
	}
	for(i=0;i<n;i++)	{
		ch = chinfo_get_by_uid(ci, uidlist[i]);
		if(ch == NULL)	{
			perror("chid NULL when batch processing update!\n");
			continue;
		}
		pre[i] = ch->chid;
	}

	culist = (channel_update**)malloc(n*sizeof(channel_update*));
	nculist = 0;

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
		culist[nculist++] = cu;
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


/*------- channel_update ops --------------------------*/
channel_update* channel_update_create()
{
	channel_update* cu;
	cu = (channel_update*)malloc(sizeof(channel_update));
	cu->leave = userlist_create();
	cu->join = userlist_create();
}

void channel_update_add_join(channel_update* cu, int uid)
{
	userlist_add(cu->join, uid);
}

void channel_update_add_leave(channel_update* cu, int uid)
{
	userlist_add(cu->leave, uid);
}
