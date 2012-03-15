#ifndef _SM_SIM_CH_
#define _SM_SIM_CH_

#include "user.h"
#include "alisttpl.h"

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

/*
typedef struct ch_info	{
	ch** chlist;	//mmm: changed from * to **, ok?
	int size;
	int capacity;
}ch_info;
*/
alisttpl_struct(ch)
typedef ch_alist ch_info;


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


/*------- chinfo ops --------------------------*/
/*
ch_info* ch_info_create();
void ch_info_add(ch_info* ci, ch* c);
*/
void ch_alist_get_update_list(ch_alist* ci, int* uidlist, int* chlist, int n, ch_update** culist, int* nculist);
ch* ch_alist_get_by_uid(ch_alist* ci, int uid);
ch* ch_alist_get_by_sgid_and_chid(ch_alist* ci, int sgid, int chid);
ch_info_client* ch_info_client_create();

/*------- channel_update ops --------------------------*/
ch_update* ch_update_create();
void ch_update_add_join(ch_update* cu, int uid);
void ch_update_add_leave(ch_update* cu, int uid);
#endif
