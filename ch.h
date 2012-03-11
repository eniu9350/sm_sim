#ifndef _SM_SIM_CH_
#define _SM_SIM_CH_

#include "user.h"

/*------- channel --------------------------*/
typedef struct freq	{
}freq;

typedef struct channel	{
	//key = sgid+chid	
	int sgid;
	int chid;

	//value = freq+usrlist
	freq* f;
	userlist* users;
}channel;

typedef struct channel_info	{
	channel* chlist;
	int size;
}channel_info;

/*------- channel update --------------------------*/
typedef struct channel_update	{
	int sgid;	//not used
	int chid;

	userlist* leave;
	userlist* join;

	int processed;	//0: not, 1: processed
}channel_update;


/*------- ch ops --------------------------*/
int channel_join(channel* ch, int uid);
int channel_leave(channel* ch, int uid);
void channel_info_get_update_list(channel_info* ci, int* uidlist, int* chlist, int n, channel_update** culist, int* nculist);
channel* channel_info_get_by_uid(channel_info* ci, int uid);
channel* channel_info_get_by_sgid_and_chid(channel_info* ci, int sgid, int chid);



/*------- channel_update ops --------------------------*/
channel_update* channel_update_create();
void channel_update_add_join(channel_update* cu, int uid);
void channel_update_add_leave(channel_update* cu, int uid);
#endif
