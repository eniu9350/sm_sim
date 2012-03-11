#ifndef _SM_SIM_SM_CLIENT_
#define _SM_SIM_SM_CLIENT_

#include "common.h"
#include "ch.h"

#define ET_CLIENT_HB_REQ 0x10
#define ET_CLIENT_HB_RSP 0x11
#define ET_CLIENT_BC_REQ 0x12
#define ET_CLIENT_BC_RSP 0x13
#define ET_CLIENT_CHCHANGE 0x14
//#define ET_CLIENT_SRV_REQ 4


#define CHANNEL_INFO_UPDATE_MODE_OVERWRITE 0
#define CHANNEL_INFO_UPDATE_MODE_APPEND 1

#define CLIENT_STATE_WORKING 0
#define CLIENT_STATE_END 1


//struct ev;

typedef struct channel_client	{
	int chid;

	freq* f;
	long ts;	//timestamp
}channel_client;


typedef struct channel_info_client	{
	channel_client* chlist;
	int size;
}channel_info_client;


typedef struct client	{
	int id;

	context_global* gctx;

	int ci_update_mode;
	channel_info_client* ci;

	int chid;	//current watching

	int state;

}client;


/*------- channel ops--------------------------*/
channel_client* channel_info_client_get(channel_info_client* ci, int chid);
void channel_info_client_update(channel_info_client* ci, channel_client* chlist, int size, int mode);


/*------- event handler--------------------------*/
void client_handle_chchange(struct ev* e);
void client_handle_bcreq(struct ev* e);
#endif
