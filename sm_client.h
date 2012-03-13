#ifndef _SM_SIM_SM_CLIENT_
#define _SM_SIM_SM_CLIENT_


#include "ev.h"
#include "ch.h"

//#define ET_CLIENT_SRV_REQ 4


#define CHANNEL_INFO_UPDATE_MODE_OVERWRITE 0
#define CHANNEL_INFO_UPDATE_MODE_APPEND 1

#define CLIENT_STATE_WORKING 0
#define CLIENT_STATE_END 1


//struct ev;
typedef struct sm_client_plan_switching_entry	{
	int chid;
	long lasting;
}sm_client_plan_switching_entry;

typedef struct sm_client_plan	{
	long arrival;	//arrival time (poweron time)
	sm_client_plan_switching_entry** switchings;
	int nswitchings;
	long departure;	//mmm: the time when last switching ends, just for convinience
}sm_client_plan;

typedef struct sm_client	{
	int id;

	//planed switching event
	sm_client_plan* plan;

	//context_global* gctx;

	int ci_update_mode;
	ch_info_client* ci;

	int chid;	//current watching

	int state;

}sm_client;


/*------- channel ops--------------------------*/
ch_client* ch_info_client_get(ch_info_client* ci, int chid);
void ch_info_client_update(ch_info_client* ci, ch_client** chlist, int size, int mode);


/*------- event handler--------------------------*/
/*
void client_handle_chchange(ev* e);
void client_handle_bcreq(struct ev* e);
*/
#endif
