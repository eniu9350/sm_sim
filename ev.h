#ifndef _SM_SIM_EV_
#define _SM_SIM_EV_

#define ET_SERVER_HB_REQ 0x00
#define ET_SERVER_HB_RSP 0x01
#define ET_SERVER_BC_REQ 0x02
#define ET_SERVER_BC_RSP 0x03
#define ET_SERVER_SRV_REQ 0x04
#define ET_SERVER_RA_REQ 0x05	//res alloc
#define ET_SERVER_RR_REQ 0x06	//res release
#define ET_SERVER_CHECK_HB 0x07

#define ET_CLIENT_HB_REQ 0x10
#define ET_CLIENT_HB_RSP 0x11
#define ET_CLIENT_BC_REQ 0x12
#define ET_CLIENT_BC_RSP 0x13
#define ET_CLIENT_CHCHANGE 0x14


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
typedef struct evdata_hbreq	{
	int uid;
	int ch;	//channel current watching
}evdata_hbreq;

typedef struct evdata_hbrsp	{
	int code;
}evdata_hbrsp;

typedef struct evdata_bcreq	{
	//int* chidlist;	//channel current serving
	//int chidsize;	//count of channels
	struct ch_client* chlist;
	int chsize;
	long life;	//validity time	//mmm: not used yet
}evdata_bcreq;

typedef struct evdata_bcrsp	{
	int code;
}evdata_bcrsp;

typedef struct evdata_srvreq	{
	int uid;
	int ch;
}evdata_srvreq;

typedef struct evdata_srvreq evdata_chchange;

typedef struct evdata_rareq	{
	int sgid;
	int chid;
}evdata_rareq;

typedef struct evdata_rrreq	{
	int sgid;
	int chid;
}evdata_rrreq;

typedef struct evdata_checkhb	{
}evdata_checkhb;	


/*------- event list --------------------------*/
//a sorted,uni directional list (by time, ascending)
typedef struct ev_list	{
	ev* head;
}ev_list;	//what's the char of list op? comparing to stack and queue? where to get & where to put?

/*------- event handler table--------------------------*/
//event code as index
typedef struct ev_handler_table	{
	void* handlers[1024];
}ev_handler_table;


/*------- event ops--------------------------*/
ev* ev_create(int type, long time);
void ev_destroy(ev* e);

/*------- event list ops--------------------------*/
ev_list* ev_list_create();
ev* ev_list_get(ev_list* l, int i);
void ev_list_gets_by_time(ev_list* l, long start, long end, ev** evlist, int* size);
int ev_list_remove(ev_list* l, int i);
int ev_list_add(ev_list* l, ev* e);

#endif
