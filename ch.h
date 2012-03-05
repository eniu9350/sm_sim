#ifndef _SM_SIM_CH_
#define _SM_SIM_CH_

/*------- ch --------------------------*/
typedef struct freq	{
}freq;

typedef struct userlist	{
	int* list;
	int size;
	int capacity;
}userlist;

typedef struct channel	{
	//key = sgid+chid	
	int sgid;
	int chid;

	//value = freq+usrlist
	freq* f;
	userlist* users;
}channel;

typedef struct channel_update	{
	int sgid;	//not used
	int chid;

	userlist* leave;
	userlist* join;
}channel_update;


typedef struct channel_info	{
	channel* chlist;
	int size;
}channel_info;


/*------- userlist ops --------------------------*/
userlist* userlist_create();
int userlist_add(userlist* ul, int uid);
int userlist_expand(userlist* ul);

/*------- ch ops --------------------------*/
channel_info_update_batch(channelinfo* ci, int* uidlist, int* chlist, int n);


/*------- channel_update ops --------------------------*/
channel_update* channel_update_create();
void channel_update_add_join(channel_update* cu, int uid);
void channel_update_add_leave(channel_update* cu, int uid);
#endif
