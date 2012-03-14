#ifndef _SM_SIM_CH_
#define _SM_SIM_CH_

#include "user.h"

/*------- channel --------------------------*/
typedef struct freq	{
}freq;

typedef struct ch	{
	//key = sgid+chid	
	int sgid;
	int chid;

	//value = freq+usrlist
	freq* f;
	userlist* users;
}ch;

typedef struct ch_client	{
	//mmm: current no sgid
	int chid;

	freq* f;
	long ts;	//timestamp
}ch_client;

typedef struct ch_info	{
	ch** chlist;	//mmm: changed from * to **, ok?
	int size;
	int capacity;
}ch_info;

typedef struct ch_info_client	{
	ch_client** chlist;	//mmm: chaned from * to **, ok?
	int size;
}ch_info_client;


/*------- channel update --------------------------*/
typedef struct ch_update	{
	int sgid;	//not used
	int chid;

	userlist* leave;
	userlist* join;

	int processed;	//0: not, 1: processed
}ch_update;


/*------- ch ops --------------------------*/
int ch_join(ch* c, int uid);
int ch_leave(ch* c, int uid);
void ch_info_get_update_list(ch_info* ci, int* uidlist, int* chlist, int n, ch_update** culist, int* nculist);
ch* ch_info_get_by_uid(ch_info* ci, int uid);
ch* ch_info_get_by_sgid_and_chid(ch_info* ci, int sgid, int chid);


/*------- chinfo ops --------------------------*/
ch_info* ch_info_create();
void ch_info_add(ch_info* ci, ch* c);

/*------- channel_update ops --------------------------*/
ch_update* ch_update_create();
void ch_update_add_join(ch_update* cu, int uid);
void ch_update_add_leave(ch_update* cu, int uid);
#endif
