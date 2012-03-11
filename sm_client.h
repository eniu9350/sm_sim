#ifndef _SM_SIM_SM_CLIENT_
#define _SM_SIM_SM_CLIENT_


#include "ch.h"

//#define ET_CLIENT_SRV_REQ 4


#define CHANNEL_INFO_UPDATE_MODE_OVERWRITE 0
#define CHANNEL_INFO_UPDATE_MODE_APPEND 1

#define CLIENT_STATE_WORKING 0
#define CLIENT_STATE_END 1


//struct ev;

typedef struct ch_client	{
	int chid;

	freq* f;
	long ts;	//timestamp
}ch_client;


typedef struct ch_info_client	{
	ch_client* chlist;
	int size;
}ch_info_client;


typedef struct sm_client	{
	int id;

	//context_global* gctx;

	int ci_update_mode;
	ch_info_client* ci;

	int chid;	//current watching

	int state;

}sm_client;


/*------- channel ops--------------------------*/
ch_client* ch_info_client_get(ch_info_client* ci, int chid);
void ch_info_client_update(ch_info_client* ci, ch_client* chlist, int size, int mode);


/*------- event handler--------------------------*/
void client_handle_chchange(struct ev* e);
void client_handle_bcreq(struct ev* e);
#endif
