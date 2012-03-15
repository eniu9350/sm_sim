#ifndef _SM_SIM_EV_
#define _SM_SIM_EV_

#include "ch.h"


#define ET_SERVER_HB_REQ 0x00
#define ET_SERVER_HB_RSP 0x01
#define ET_SERVER_BC_REQ 0x02
#define ET_SERVER_BC_RSP 0x03
#define ET_SERVER_SRV_REQ 0x04
#define ET_SERVER_RA_REQ 0x05	//res alloc
#define ET_SERVER_RR_REQ 0x06	//res release
#define ET_SERVER_CHECK_HB 0x07

#define ET_CLIENT_POWER_ON 0x10
#define ET_CLIENT_HB_REQ 0x11
#define ET_CLIENT_HB_RSP 0x12
#define ET_CLIENT_BC_REQ 0x13
#define ET_CLIENT_BC_RSP 0x14
#define ET_CLIENT_SWITCHING 0x15
#define ET_CLIENT_SRV_REQ 0x16


/*------- event --------------------------*/
typedef struct ev	{
	int type;
	long time;
	void* data;
	void* agent;	//event listener info 
	int processed;	//0: not processed, 1: processed
	struct ev* next;
}ev;	//dyn info

/*------- data of concrete event types --------------------------*/
/*------- server event types--------------------------*/
typedef struct evdata_server_hb_req	{
	int uid;
	int chid;	//channel current watching
}evdata_server_hb_req;

/*
typedef struct evdata_server_hb_req_list	{
	struct evdata_server_hb_req** list;
	int size;
	int capacity;
}evdata_server_hb_req_list;
*/
alisttpl_struct(evdata_server_hb_req)

typedef struct evdata_server_hb_rsp	{
	int code;
}evdata_server_hb_rsp;

typedef struct evdata_server_bc_req	{
}evdata_server_bc_req;	//just a notification

typedef struct evdata_server_ra_req	{
	int sgid;
	int chid;
}evdata_server_ra_req;

typedef struct evdata_server_rr_req	{
	int sgid;
	int chid;
}evdata_server_rr_req;

typedef struct evdata_server_check_hb	{
}evdata_server_check_hb;	

typedef struct evdata_server_srv_req	{
	int uid;
	int chid;
}evdata_server_srv_req;
/*------- client event types--------------------------*/
//just a notification for client to send hb req, chid is included in server event
typedef struct evdata_client_hb_req	{
//	int uid;
//	int ch;	//channel current watching
}evdata_client_hb_req;

typedef struct evdata_client_bc_req	{
	ch_client** chlist;
	int size;
}evdata_client_bc_req;

//mmmm: not used yet
typedef struct evdata_client_srv_req	{
	int uid;
	int chid;
}evdata_client_srv_req;

typedef struct evdata_client_switching{
	int chid;
}evdata_client_switching;





/*------- event list --------------------------*/
//a sorted,uni directional list (by time, ascending)
typedef struct ev_list	{
	ev* head;
}ev_list;	//what's the char of list op? comparing to stack and queue? where to get & where to put?

/*------- event handler table--------------------------*/
//event code as index
//typedef void* pvoid;


/*------- event ops--------------------------*/
ev* ev_create(int type, long time);
void ev_destroy(ev* e);

/*------- event list ops--------------------------*/
ev_list* ev_list_create();
ev* ev_list_get(ev_list* l, int i);
ev* ev_list_pop_head(ev_list* l);	//mmmm: exception handling not considered
void ev_list_gets_by_time(ev_list* l, long start, long end, ev** evlist, int* size);
static ev* ev_list_remove_without_destroy(ev_list* l, int i, int* result);
int ev_list_remove(ev_list* l, int i);
int ev_list_add(ev_list* l, ev* e);

/* -------- */
#endif
