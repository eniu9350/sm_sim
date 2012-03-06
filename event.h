#ifndef _SM_SIM_EVENT_
#define _SM_SIM_EVENT_

#include "ch.h"
#include "client.h"


/*------- server event type --------------------------*/
#define ET_HB_REQ 0x00
#define ET_HB_RSP 0x01
#define ET_BC_REQ 0x02
#define ET_BC_RSP 0x03
#define ET_SRV_REQ 0x04

#define ET_ES_REQ 0x05

#define ET_CHECK_HB 0x06

struct channel_client;


/*------- event and event queue--------------------------*/
typedef struct ev	{
	int type;
	long time;
	void* data;
	void* agent;	//event listener info 
	int processed;	//0: not processed, 1: processed
	struct ev* next;
}ev;	//dyn info

//a sorted list (by time, ascending)
typedef struct ev_list	{
	ev* head;
}ev_list;	//what's the char of list op? comparing to stack and queue? where to get & where to put?


/*------- event handler table--------------------------*/
typedef struct evhandler_table	{
	void* handlers[1024];
}evhandler_table;

/*------- event loop--------------------------*/
typedef struct ev_loop	{
	long now;
	evhandler_table* ht;
	ev_list* evlist;
}ev_loop;

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
	struct channel_client* chlist;
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

typedef evdata_srvreq evdata_chchange;


/*------- event loop ops--------------------------*/
void ev_loop_loop(context_global* gctx, ev_loop* el);

/*------- event ops--------------------------*/
ev* ev_create(int type, long time);
void ev_destroy(ev* e);

/*------- event list ops--------------------------*/
ev_list* ev_list_create();
ev* ev_list_get(ev_list* l, int i);
void ev_list_gets_by_time(ev_list* l, long start, long end, ev** evlist, int* size);
int ev_list_remove(ev_list* l, int i);
int ev_list_add(ev_list* l, ev* e);

//originally, fire_event should be executing handler instantaneously, but we allows to postponing event here for convinience
int fire_event(ev_list* l, ev* e);	//wrapper of ev_list_add

void reg_event_handler(ev_loop* l, int type, void* handler);
#endif
